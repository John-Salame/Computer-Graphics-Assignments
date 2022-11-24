import numpy as np
import matplotlib.pyplot as plt
import cv2
import sys

# Assumption is that gray areas are indented and white areas are just normal.
# This assumption works for snow and candy cane.

# white stripes take a value equal to the average of the area minus 20
def convert_white_stripes_to_indent(k, img):
    # k is the radius of kernel for average
    img = img.astype('double')
    shape = img.shape
    w = shape[1]
    h = shape[0]
    for i in range(k, h-k):
        for j in range(k, w-k):
            if img[i,j] > 200:
                img[i,j] = np.average(img[i-k:i+k,j-k:j+k]) - 20
    return img.astype('uint32')

# if the right side is brighter, normal is to left (negative red). Thus, we subtract pixel intensities on the right and add intensities on the left.
# the kernel is size k (radius)
def red_normal(k, img):
    # first, create the margins
    shape = img.shape
    w = shape[1]
    h = shape[0]
    img_pad = np.zeros((h+2*k, w+2*k), dtype=float)
    w = img_pad.shape[1]
    h = img_pad.shape[0]
    # duplicate image to center of padded image
    img_pad[k:(h-k), k:(w-k)] = img.astype('float')
    # duplicate first and last rows to padding
    for i in range(k):
        img_pad[i, :] = img_pad[k, :]
        img_pad[h-1-i, :] = img_pad[h-k-1, :]
    # duplicate first and last columns to padding
    for j in range(k):
        img_pad[:, j] = img_pad[:, k]
        img_pad[:, w-1-j] = img_pad[:, w-k-1]
    
    # Now that we have our padded image, normalize the image
    my_avg = np.average(img_pad)
    # normalize to [-1.0, 1.0] with the mean at 0
    img_pad = img_pad - my_avg
    my_max = np.amax(img_pad)
    my_min = np.amin(img_pad)
    my_range = my_max - my_min
    # img_pad[img_pad > 0] = img_pad[img_pad > 0] / my_max
    # img_pad[img_pad < 0] = -1.0 * img_pad[img_pad < 0] / my_min
    img_pad = img_pad / my_range
    print('max intensity %f' % np.amax(img_pad))
    print('min intensity %f' % np.amin(img_pad))
    print('avg intensity %f' % np.average(img_pad))

    # create the kernel
    # The kernel should be (k*2+1 by k*2+1), but I will make int only one row wide and use a trick
    coef = 1 / (2*k+1)
    kernel = np.zeros((1, 2*k+1))
    kernel[:, 0:k] = coef
    kernel[:, k+2:] = -1 * coef
    # make it compatible with matrix multiplication
    # kernel = np.transpose(kernel)

    # apply the kernel to columns of the padded matrix
    red_img = np.zeros(img_pad.shape)
    for j in range(w-2*k):
        cur_cols = img_pad[:, j:(j+1+2*k)]
        # red_img[:, j+k] = np.sum(np.multiply(kernel, img_pad[:, j:(j+1+2*k)]), axis=1)
        red_img[:, j+k] = np.sum(img_pad[:, j:(j+1+2*k)] * kernel, axis=1)
        # red_img[:, j+k] = np.matmul(img_pad[:, j:(j+1+2*k)], kernel)
    print('max red %f' % np.amax(red_img))
    print('min red %f' % np.amin(red_img))
    print('avg red %f' % np.average(red_img))
    return red_img

def main():
    img = plt.imread(in_file)
    k = 3 # radius of the kernel for various things
    print(type(img))
    print(img.shape)
    # keep intensities only (b here stands for brightness)
    img_b = np.zeros((img.shape[0], img.shape[1]), dtype=np.uint32)
    img_b = 0.33*img[:,:,0] + 0.33*img[:,:,1] + 0.33*img[:,:,2]
    # Create a depression where the white slash on the texture is
    if in_file == 'candyCane.bmp':
        img_b = convert_white_stripes_to_indent(k, img_b)
    img_b = red_normal(k, img_b)
    print(img_b.shape)
    print(img_b[150, :])
    plt.subplot(1, 2, 1)
    plt.imshow(img)
    plt.title('Original texture')
    plt.subplot(1, 2, 2)
    # plt.imshow(img_b, cmap=plt.get_cmap('gray'))
    plt.imshow(img_b, cmap=plt.colormaps['Reds'])
    plt.colorbar()
    plt.title('Grayscale')
    plt.show()
    return 0

if __name__ == "__main__":
    if len(sys.argv) < 1:
        raise Exception("Missing command line argument for input file name")
        sys.exit(1)
    in_file = sys.argv[1]
    exit_status = main()
    sys.exit(exit_status)
