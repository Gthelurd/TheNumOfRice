% opengl('save','software') %因为我的linux电脑太老了，不支持nvida硬件加速，只能软件解码
clear;
close all;

i = imread('rice.png');
BG = imopen(i, strel('disk', 15));
subplot(2, 3, 1);
imshow(BG);
title('Background');
i2 = imsubtract(i, BG);
subplot(2, 3, 2);
imshow(i2);
title('Subtracted Background');
level = graythresh(i2);
bw2 = imbinarize(i2, level);
subplot(2, 3, 3);
imshow(bw2);
title('Binary Image');
subplot(2, 3, 4);
imshow(i);
title('Original Image');

% 标记连通区域
[labeled, numObjects] = bwlabel(bw2, 8);
t = max(max(labeled));  % 大米粒计数总数为 t

% 求解大米粒的平均像素面积
s = sum(bw2(:));
area = s / t;  % 大米粒的平均像素面积 area

% 给面积最大的米粒染成红色
[m, n] = size(labeled);
A = zeros(numObjects, 1);
for x = 1:numObjects
    A(x) = sum(labeled(:) == x);
end
a = max(A);
inf = find(A == a);

% 读取原始图像
image = imread('rice.png');
i3 = zeros(size(image, 1), size(image, 2), 3);
i4 = double(image);

% 灰度图转彩色图
for i = 1:size(image, 1)
    for j = 1:size(image, 2)
        i3(i, j, 1) = i4(i, j);
        i3(i, j, 2) = i4(i, j);
        i3(i, j, 3) = i4(i, j);
    end
end

subplot(2, 3, 5);
imshow(uint8(i3));
title('Color Image');

% 将面积最大的米粒染成红色
for v = 1:size(image, 1)
    for o = 1:size(image, 2)
        if labeled(v, o) == inf
            i3(v, o, 1) = 255;
            i3(v, o, 2) = 0;
            i3(v, o, 3) = 0;
        end
    end
end

subplot(2, 3, 6);
imshow(uint8(i3));
title('Largest Grain in Red');