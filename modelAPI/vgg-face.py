#!/usr/bin/env python3
# coding: utf-8

# This is python-2.7 code.


#    MIT License
#
#    Copyright (c) 2019 Sefik Ilkin Serengil (sefiks.com)
#
#    Permission is hereby granted, free of charge, to any person obtaining a copy
#    of this software and associated documentation files (the "Software"), to deal
#    in the Software without restriction, including without limitation the rights
#    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#    copies of the Software, and to permit persons to whom the Software is
#    furnished to do so, subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in all
#    copies or substantial portions of the Software.
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#    SOFTWARE.

import shutil
import sys
import os
from keras.models import Model, Sequential, model_from_json, load_model
from keras.layers import Input, Convolution2D, ZeroPadding2D, MaxPooling2D, Flatten, Dense, Dropout, Activation
from PIL import Image
import numpy as np
from keras.preprocessing.image import load_img, save_img, img_to_array
from keras.applications.imagenet_utils import preprocess_input
from keras.preprocessing import image
import matplotlib.pyplot as plt

# MARK: preprocess image
"""
    preprocess_image(image_patg).
    Input: absoulute or reference path to the iamge.
    Output: preprocessed image.
    """
def preprocess_image(image_path):
    img = load_img(image_path, target_size=(224, 224))
    img = img_to_array(img)
    img = np.expand_dims(img, axis=0)
    img = preprocess_input(img)
    return img


# MARK: findCosineSimilarity
"""
    findCosineSimilarity(source_representation, test_representation).
    Input: source and test representations of face.
    Output: cosine similarity of faces.
    """
def findCosineSimilarity(source_representation, test_representation):
    a = np.matmul(np.transpose(source_representation), test_representation)
    b = np.sum(np.multiply(source_representation, source_representation))
    c = np.sum(np.multiply(test_representation, test_representation))
    return 1 - (a / (np.sqrt(b) * np.sqrt(c)))

# MARK: findCosineSimilarity
"""
    findEuclideanDistance(source_representation, test_representation).
    Input: source and test representations of face.
    Output: euclidean distance of faces.
    """
def findEuclideanDistance(source_representation, test_representation):
    euclidean_distance = source_representation - test_representation
    euclidean_distance = np.sum(np.multiply(euclidean_distance, euclidean_distance))
    euclidean_distance = np.sqrt(euclidean_distance)
    return euclidean_distance


# MARK: model activation
"""
    loadFaceDescriptor().
    Activates model and loads pre-trained weights.
    They should be put in the same directory with script.
    
    Output: vgg face descriptor.
    """
def loadFaceDescriptor():
    # model layers initialization
    model = Sequential()
    model.add(ZeroPadding2D((1,1),input_shape=(224,224, 3)))
    model.add(Convolution2D(64, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D((2,2), strides=(2,2)))

    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(128, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(128, (3, 3), activation='relu'))
    model.add(MaxPooling2D((2,2), strides=(2,2)))

    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(256, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(256, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(256, (3, 3), activation='relu'))
    model.add(MaxPooling2D((2,2), strides=(2,2)))

    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(512, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(512, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(512, (3, 3), activation='relu'))
    model.add(MaxPooling2D((2,2), strides=(2,2)))

    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(512, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(512, (3, 3), activation='relu'))
    model.add(ZeroPadding2D((1,1)))
    model.add(Convolution2D(512, (3, 3), activation='relu'))
    model.add(MaxPooling2D((2,2), strides=(2,2)))

    model.add(Convolution2D(4096, (7, 7), activation='relu'))
    model.add(Dropout(0.5))
    model.add(Convolution2D(4096, (1, 1), activation='relu'))
    model.add(Dropout(0.5))
    model.add(Convolution2D(2622, (1, 1)))
    model.add(Flatten())
    model.add(Activation('softmax'))
    
    base_dir = os.path.dirname(os.path.abspath(__file__))
    weights_path = base_dir + '/vgg_face_weights.h5'
    # loading weights. They should be put in the same directory with script.
    model.load_weights(weights_path)
    
    _vgg_face_descriptor = Model(inputs=model.layers[0].input, outputs=model.layers[-2].output)
    _vgg_face_descriptor.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    _vgg_face_descriptor.save('recognition_model.h5')
    # return _vgg_face_descriptor


# MARK: verifyFace
"""
    verifyFace(absolute_path_img1, absolute_path_img2, vgg_face_descriptor).
    Input: absolute paths of 2 faces images to compare and vgg_face_descriptor as a result of call of loadFaceDescriptor function.
    Output: 1 - if the same person, 0 - if not.
    """
def verifyFace(absolute_path_img1, absolute_path_img2, vgg_face_descriptor):
    # threshold value
    epsilon = 0.36
    
    img1_representation = vgg_face_descriptor.predict(preprocess_image('%s' % (absolute_path_img1)))[0,:]
    img2_representation = vgg_face_descriptor.predict(preprocess_image('%s' % (absolute_path_img2)))[0,:]
    
    cosine_similarity = findCosineSimilarity(img1_representation, img2_representation)
    euclidean_distance = findEuclideanDistance(img1_representation, img2_representation)
    
    if(cosine_similarity < epsilon):
        return [1, cosine_similarity]
    else:
        return [0, cosine_similarity]



# return types:
# 1 - unknown person
# 2 - invalid number of arguments
# person id

argv = sys.argv
if(len(argv) != 3):
    sys.stdout.write('2' + '\n')
else:
    base_dir = os.path.dirname(os.path.abspath(__file__))
    vgg_face_descriptor = load_model(base_dir + '/recognition_model.h5')
    template_dir = argv[1]
    target_img_dir = argv[2]

    for target_img_file in sorted(os.listdir(target_img_dir)):
        image_id, file_extension = os.path.splitext(target_img_file)
        if (file_extension not in ['.png','.JPG','.jpg','.jpeg', '.JPEG', '.PNG']):
            continue
        result = 0

        cosine_similarity = 1
        for template in sorted(os.listdir(template_dir)):
            absolute_path_img_template = template_dir + template
            absolute_path_img = target_img_dir + target_img_file

            res = verifyFace(absolute_path_img, absolute_path_img_template, vgg_face_descriptor)
            if res[0] == 1:
                if res[1] < cosine_similarity:
                    cosine_similarity = res[1]
                    image_id, file_extension = os.path.splitext(template)
                    result = 1

        if result == 0:
            sys.stdout.write('1' + '\n')
        if result == 1: 
            sys.stdout.write(image_id + '\n')

    shutil.rmtree(target_img_dir, ignore_errors=True)