%% example.m
% Writen by https://blog.csdn.net/weixin_50731917/article/details/128463253

clear, clc, close all

%载入图片
srcImage=imread('rice.png');

im = rgb2gray(srcImage);
bw = imbinarize(im, graythresh(im));

%运用开操作消去噪点
se = strel('disk',10);
openbw = imopen(bw,se);

% 形态学处理，腐蚀图像
se = strel('disk',15);
openbw = imerode(openbw,se);  

%获取连通区域，并进行显示
L = bwlabel(openbw,8);
RGB = label2rgb(L);

% 求各个连通区域的凸包
convex_img = zeros(size(im));
status = regionprops(L, 'ConvexHull');
for i = 1:size(status,1)
     tn = uint16(status(i).ConvexHull);
     convex_img = convex_img + roipoly(im,tn(:, 1),tn(:, 2));
end

% 凸包与凸包处理前做差处理
diff_img = convex_img-openbw;

% 运用开操作消去噪点
se = strel('disk',5);
diff_open = imopen(diff_img,se); 

% 运用闭操作填补轮廓
se = strel('disk',60);
diff_close = imclose(diff_open,se); 

% 闭运算结果与原二值图做差
diff_img = openbw-diff_close;

% 获取连通区域，并进行显示
diff_img = imbinarize(diff_img, graythresh(diff_img));
[L,num] = bwlabel(diff_img, 8);
status = regionprops(L,'BoundingBox'); % 获取标记框
centroid = regionprops(L,'Centroid'); % 获取标记中心
RGB = label2rgb(L);

% 最终显示结果
figure, imshow(srcImage), title(sprintf("大米数量为%d",num));
hold on;
for n = 1:num
	rectangle('position',status(n).BoundingBox,'edgecolor','r'); % 画框
    text(centroid(n,1).Centroid(1,1)-1,centroid(n,1).Centroid(1,2)-1, num2str(n),'Color', 'r')  % 标号
end

