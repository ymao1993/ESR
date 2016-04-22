%A helper script to output the bounding boxes data into txt file

outputFileName = 'bbox_lfpw_train.txt';
numImage = size(bounding_boxes,2);
fileID = fopen(outputFileName, 'W');
for i = 1:numImage
     disp(bounding_boxes{i}.imgName);
     bbox = bounding_boxes{i}.bb_ground_truth;
     fprintf(fileID,'%.4f %.4f %.4f %.4f\n',bbox(1),bbox(2),bbox(3),bbox(4));
end
fclose(fileID);
