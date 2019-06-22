# naive_patch_correspondence

Many popular computer vision and graphics algorithms rely on nearest neighbor fields, i.e. correspondences between small neighborhoods within images. This is an exhaustive algorithm for finding optimal nearest neighborfields. 

NOTE: Though this technique guarantees correctness, it is very slow. Approximation methods, e.g. Propagation-Assisted KD-Trees, are often sufficiently accurate for many applications and are significantly faster. This code is not intended for practical use, but for optimality comparison purposes. 

