import numpy as np
import cv2
import math
import random

from numpy.core.fromnumeric import resize


def RANSACFilter(
        matched_pairs, keypoints1, keypoints2,
        orient_agreement, scale_agreement):
    assert isinstance(matched_pairs, list)
    assert isinstance(keypoints1, np.ndarray)
    assert isinstance(keypoints2, np.ndarray)
    assert isinstance(orient_agreement, float)
    assert isinstance(scale_agreement, float)
    ## START
    # 빈 largest_set을 생성합니다
    largest_set = []

    for _ in range(10):
        # random.choice 함수를 이용해 matched_pairs 중 무작위의 match 쌍 하나를 선택하고,
        # 이것을 비교 기준으로 삼을 것입니다.
        # orientation과 scale 비교를 위해, 이 기준 match 쌍의
        # difference of orientation과 change of scale을 계산합니다.
        selected_match = random.choice(matched_pairs)
        # difference of orientation의 경우 두 orientation 차이의 절댓값을 취합니다.
        initial_orient_diff = abs(keypoints2[selected_match[1]][3]-keypoints1[selected_match[0]][3])
        initial_scale_change = keypoints2[selected_match[1]][2] / keypoints1[selected_match[0]][2]

        # 기준 match 쌍을 포함한 전체 match쌍에 대해,
        # difference of orientation과 change of scale의 차이 혹은 비율이
        # 모두 각각의 agreement 범위 내에 있을 경우 consistency를 갖는다고 보고
        # candidate_set에 포함시킵니다.
        candidate_set = []
        for match in matched_pairs:
            orient_diff = abs(keypoints2[match[1]][3]-keypoints1[match[0]][3])
            scale_change = keypoints2[match[1]][2] / keypoints1[match[0]][2]
            orient_cond = (abs (orient_diff - initial_orient_diff) <= math.radians(orient_agreement))
            scale_cond = (initial_scale_change*(1.0-scale_agreement) <= scale_change <= initial_scale_change*(1.0+scale_agreement))
            if(orient_cond and scale_cond):
                candidate_set.append(match)
        
        # 만약 조건을 만족하는 consistency set의 크기가 현재까지 set 중에 가장 크다면
        # 이것을 largest_set으로 삼습니다.
        if(len(candidate_set) > len(largest_set)):
            largest_set = candidate_set

    ## END
    assert isinstance(largest_set, list)
    return largest_set



def FindBestMatches(descriptors1, descriptors2, threshold):
    assert isinstance(descriptors1, np.ndarray)
    assert isinstance(descriptors2, np.ndarray)
    assert isinstance(threshold, float)
    ## START
    ## the following is just a placeholder to show you the output format
    
    # 빈 matched_pairs 리스트를 생성합니다.
    matched_pairs = []

    # 2번째 이미지의 각각의 descriptor에 대해 가장 잘 맞는 match를 찾습니다.
    for j, desc2 in enumerate(descriptors2):
        # descriptors1의 모든 descriptor vector들과 현재 descriptor2를 내적한 뒤
        # 이를 np.arccos 함수에 대입하여 각도 배열을 얻어냅니다.
        angles = np.arccos(np.dot(descriptors1, desc2))

        # angles 배열을 정렬한 뒤,
        # 그 첫 번째 값(best match)과 두 번째 값(second best match)의 비율이
        # threshold보다 작은 경우에만 유효한 match인 것으로 판단하고
        # matched_pairs에 추가합니다.
        # 이 때 best_match_index는 descriptors1의 원소를 가리키고,
        # j는 descriptors2의 원소를 가리키므로
        # 순서에 유의하여 matched_pair에 추가합니다.
        sorted_angles = sorted(angles)
        if((sorted_angles[0]/sorted_angles[1]) <= threshold):
            best_match_index = np.where(angles == sorted_angles[0])[0][0]
            matched_pairs.append([best_match_index, j])
    ## END
    return matched_pairs


def KeypointProjection(xy_points, h):
    assert isinstance(xy_points, np.ndarray)
    assert isinstance(h, np.ndarray)
    assert xy_points.shape[1] == 2
    assert h.shape == (3, 3)
    # START
    # 전달받은 xy_points 배열의 오른쪽에 한 열을 더하고, 그 값을 1로 채워줍니다.
    xy_points_out = np.concatenate((xy_points, np.zeros((xy_points.shape[0], 1))), axis=1)
    xy_points_out[:,2] = 1

    # 원래 Homography를 적용하기 위해서는 h*coordinate 순서로 행렬을 곱해줘야 하지만,
    # 입력 받은 좌표의 방향과 출력 좌표의 방향이 원래A 방향에서 transpose된 것과 같으므로
    # 여기서는 homography도 같이 transpose한 뒤 곱셈의 순서도 바꾸었습니다.
    xy_points_out = np.dot(xy_points_out, h.T)

    # 행렬곱의 결과 중 추가 열의 값이 0인 경우에는 1e-10으로 바꿉니다.
    xy_points_out[:,2][xy_points_out[:,2] == 0] = 1e-10

    # 각각의 좌표를 추가 열의 값으로 나누어
    # homogeneous 좌표를 2d point 좌표로 변환해줍니다.
    # 또한 반환 형식을 지키기 위해 추가 열은 제외한
    # num_points * 2 크기의 배열을 최종적으로 반환합니다.
    xy_points_out = xy_points_out / xy_points_out[:,2:]
    xy_points_out = xy_points_out[:,0:2]

    # END
    return xy_points_out

def RANSACHomography(xy_src, xy_ref, num_iter, tol):
    """
    Given matches of keypoint xy coordinates, perform RANSAC to obtain
    the homography matrix. At each iteration, this function randomly
    choose 4 matches from xy_src and xy_ref.  Compute the homography matrix
    using the 4 matches.  Project all source "xy_src" keypoints to the
    reference image.  Check how many projected keyponits are within a `tol`
    radius to the coresponding xy_ref points (a.k.a. inliers).  During the
    iterations, you should keep track of the iteration that yields the largest
    inlier set. After the iterations, you should use the biggest inlier set to
    compute the final homography matrix.
    Inputs:
        xy_src: a numpy array of xy coordinates, (num_matches, 2)
        xy_ref: a numpy array of xy coordinates, (num_matches, 2)
        num_iter: number of RANSAC iterations.
        tol: float
    Outputs:
        h: The final homography matrix.
    """
    assert isinstance(xy_src, np.ndarray)
    assert isinstance(xy_ref, np.ndarray)
    assert xy_src.shape == xy_ref.shape
    assert xy_src.shape[1] == 2
    assert isinstance(num_iter, int)
    assert isinstance(tol, (int, float))
    tol = tol*1.0

    # START
    # 현재까지의 match 수의 최댓값을 기록할 변수 largest를 0으로 초기화합니다.
    largest = 0

    for _ in range(num_iter):
        # 우선 전체 match 중 무작위로 4개의 match를 뽑습니다.
        # 무작위 인덱스를 뽑은 뒤 해당 하는 keypoint를
        # xy_src, xy_ref에서 뽑아내는 방식으로 진행합니다.
        index_sample = random.sample(range(xy_ref.shape[0]),4)
        xy_src_sample = xy_src[index_sample]
        xy_ref_sample = xy_ref[index_sample]

        # homography를 구하기 위해 xy_src_sample와 xy_ref_sample의 좌표를 이용하여 행렬 A를 만들 것입니다.
        # 이 때 A를
        # [[1 1 1 0 0 0 -x1' -x1' -x1']
        # [0 0 0 1 1 1 -y1' -y1'-y1']
        # ...] ... (1)
        # 와
        # [[x1 y1 1 x1 y1 1 x1 y1 1]
        # [x1 y1 1 x1 y1 1 x1 y1 1]
        # ...] ... (2)
        # 의 원소끼리의 곱으로 볼 수 있기 때문에, 각각의 행렬을 먼저 계산한 다음 원소끼리 곱해주면
        # 원하는 행렬 A를 얻을 수 있을 것입니다.
        # 

        # 위의 (1) 행렬을 만듭니다. 이 때 repeat, tile 등 원소를 복제하는 함수들을 사용하였습니다.
        xy_ref_modified = np.repeat(xy_ref_sample.reshape(-1,1),3, axis=1)*-1
        xy_ref_modified[0::2,0] = 1
        xy_ref_modified[1::2,0] = 0
        xy_ref_modified[1::2,1] = 1
        xy_ref_modified[0::2,1] = 0
        xy_ref_modified = np.repeat(xy_ref_modified, 3, axis=1)

        # 마찬가지로 (2) 행렬을 만듭니다. 
        xy_src_modified = np.concatenate((xy_src_sample, xy_src_sample[:,1:]), axis=1)
        xy_src_modified[:,2] = 1
        xy_src_modified = np.repeat(xy_src_modified, 2, axis=0)
        xy_src_modified = np.tile(xy_src_modified, 3)

        # A 행렬을 만들고, A.T*A의 고유값을 구하기 위해 특이값 분해 함수 svd를 사용합니다.
        A = xy_ref_modified * xy_src_modified
        u,s,vt = np.linalg.svd(np.dot(A.T, A))

        # sigma(=고유값의 제곱근)가 가장 작은 인덱스를 찾은 후
        # 그에 해당하는 고유벡터를 vt에서 찾아내고, reshape으로 재형성합니다.
        # 이 3x3 행렬이 최종 homography의 후보입니다.
        h_cand = vt[s.argmin()].reshape((3,3))
        h_cand = h_cand / h_cand[2,2]


        well_proj_points = []
        # Part 2.3 에서 작성했던 projection 함수를 이용해 xy projection point를 얻어냅니다.
        xy_proj = KeypointProjection(xy_src, h_cand)

        # projection point와 reference 간의 거리를 모두 계산하여 배열로 만듭니다.
        point_diff = xy_proj - xy_ref
        proj_distances = np.hypot(point_diff[:,0:1], point_diff[:,1:])

        # 계산된 거리 중 tol 값보다 작은 경우의 수를 산정하고
        # 이것이 지금까지 나온 어떤 경우보다 많다면
        # largest를 현재 기록으로 갱신하고, 반환할 homography h는 현재 homography로 대체합니다.
        well_proj_points = xy_proj[np.where(proj_distances<=tol)]

        if(largest <= len(well_proj_points)):
            h = h_cand
            largest = len(well_proj_points)
            

        
    # END
    assert isinstance(h, np.ndarray)
    assert h.shape == (3, 3)
    return h


def FindBestMatchesRANSAC(
        keypoints1, keypoints2,
        descriptors1, descriptors2, threshold,
        orient_agreement, scale_agreement):
    """
    Note: you do not need to change this function.
    However, we recommend you to study this function carefully
    to understand how each component interacts with each other.

    This function find the best matches between two images using RANSAC.
    Inputs:
        keypoints1, 2: keypoints from image 1 and image 2
            stored in np.array with shape (num_pts, 4)
            each row: row, col, scale, orientation
        descriptors1, 2: a K-by-128 array, where each row gives a descriptor
        for one of the K keypoints.  The descriptor is a 1D array of 128
        values with unit length.
        threshold: the threshold for the ratio test of "the distance to the nearest"
                   divided by "the distance to the second nearest neighbour".
                   pseudocode-wise: dist[best_idx]/dist[second_idx] <= threshold
        orient_agreement: in degrees, say 30 degrees.
        scale_agreement: in floating points, say 0.5
    Outputs:
        matched_pairs_ransac: a list in the form [(i, j)] where i and j means
        descriptors1[i] is matched with descriptors2[j].
    Detailed instructions are on the assignment website
    """
    orient_agreement = float(orient_agreement)
    assert isinstance(keypoints1, np.ndarray)
    assert isinstance(keypoints2, np.ndarray)
    assert isinstance(descriptors1, np.ndarray)
    assert isinstance(descriptors2, np.ndarray)
    assert isinstance(threshold, float)
    assert isinstance(orient_agreement, float)
    assert isinstance(scale_agreement, float)
    matched_pairs = FindBestMatches(
        descriptors1, descriptors2, threshold)
    matched_pairs_ransac = RANSACFilter(
        matched_pairs, keypoints1, keypoints2,
        orient_agreement, scale_agreement)
    return matched_pairs_ransac
