#!/usr/bin/env python3
# Import libraries
import sys
import os
import cv2
import numpy as np
import tempfile

# Define paths
base_dir = os.path.dirname(os.path.abspath(__file__))
prototxt_path = os.path.join(base_dir + '/deploy.prototxt')
caffemodel_path = os.path.join(base_dir + '/weights.caffemodel')

# Read the model
model = cv2.dnn.readNetFromCaffe(prototxt_path, caffemodel_path)

# Securely create temporary directory. vgg-face.py is responsible for removing after finished work.
tmp_dir = tempfile.mkdtemp()

def get_face_rects(image_path):
	result = [];
	coords = [];
	image_id = 0
	file_name, file_extension = os.path.splitext(image_path)
	if (file_extension in ['.png','.JPG','.jpg','.jpeg']):
		image = cv2.imread(image_path)

		(h, w) = image.shape[:2]
		blob = cv2.dnn.blobFromImage(cv2.resize(image, (300, 300)), 1.0, (300, 300), (104.0, 177.0, 123.0))

		model.setInput(blob)
		detections = model.forward()

		# Identify each face
		for i in range(0, detections.shape[2]):
			box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
			(startX, startY, endX, endY) = box.astype("int")

			confidence = detections[0, 0, i, 2]

			# If confidence > 0.5, save it as a separate file
			if (confidence > 0.5):
				result.append(image_id)
				result.append(startX)
				result.append(endX)
				result.append(startY)
				result.append(endY)
				image_id += 1
				frame = image[startY:endY, startX:endX]
				cv2.imwrite(tmp_dir + '/' + str(image_id) + file_extension, frame)
	return result


# return types:
# directory where images located
# vector of arrays with coordinates for each face, 
# where array starts with face unique index.
# 2 - invalid number of arguments
argv = sys.argv
if(len(argv) != 2):
    sys.stdout.write('err' + '\n')
else:
	sys.stdout.write(tmp_dir + '\n')
	res = get_face_rects(argv[1])
	for element in res:
		sys.stdout.write(str(element) + '\n')
		sys.stdout.write(',' + '\n')
	sys.stdout.write('.' + '\n')