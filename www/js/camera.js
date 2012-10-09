/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
*/

function Camera(){
   this.DestinationType = Camera.DestinationType;
   this.PictureSourceType = Camera.PictureSourceType;
   this.EncodingType = Camera.EncodingType;
   this.MediaType = Camera.MediaType;
   return this;
}

Camera.DestinationType = {
        DATA_URL : 0,                // Return image as base64 encoded string
        FILE_URI : 1                 // Return image file URI
};

Camera.PictureSourceType = {
    PHOTOLIBRARY : 0,
    CAMERA : 1,
    SAVEDPHOTOALBUM : 2
};


Camera.EncodingType = {
     JPEG : 0,               // Return JPEG encoded image
     PNG : 1                 // Return PNG encoded image
};

Camera.MediaType = {
     PICTURE : 0,
     VIDEO : 1,
     ALLMEDIA: 2
};

function CameraOptions() {
};

/*
CameraOptions.prototype.magneticHeading = 0;

quality : 75,
                                  destinationType : Camera.DestinationType.DATA_URL,
                                  sourceType : Camera.PictureSourceType.CAMERA,
                                  allowEdit : true,
                                  encodingType: Camera.EncodingType.JPEG,
                                  targetWidth: 100,
                                  targetHeight: 100
*/

Camera.prototype.getPicture = function( successCallback, errorCallback, options ) {
    // Check the callbacks
    console.log("Camera.getPicture()")
    if( typeof successCallback !== "function" ) return;
    if( typeof errorCallback !== "function" ) errorCallback = function() {};
    var cameraOptions = new CameraOptions();

    // Call the native function
    Cordova.exec( successCallback, errorCallback, "com.cordova.Camera", "getPicture", [ cameraOptions ] );
};

Cordova.addConstructor( "com.cordova.Camera", function () {
                          console.log("Camera.ctor()")
                            navigator.camera = new Camera();
                        } );
