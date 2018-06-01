# OpenCV-laser-projection-virtual-keyboard
The virtual keyboard design based on laser projection technology is based on OpenCV, it combines with hardware identification and detects the location of the finger position and then maps to the keyboard so as to realize the corresponding KeyPress. 
## How to Work？
- 1. How to Work？

At the bottom of the infrared laser emission from a surface of a covering space in the infrared range, of course this plane to cover the entire keyboard, a keyboard in the center of the keyboard is the projection contour shape of the keyboard is mainly used for calibration, at the top of the real-time camera outside the graphics and pass data to the computer, because the laser light is a horizontal and parallel so no object occlusion camera is unable to detect the infrared signal, but if there are objects in the infrared laser region when the occluded objects' surface will be covered with the infrared camera will detect the infrared signal, computer access to the signal sent by the camera after after a certain the algorithm to obtain coordinates in the picture of infrared spot and then put the coordinate mapping to real keyboard position so as to realize its function.

![How to Work](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Howtowork.jpg)

![How to Work](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/howtowork.gif)

------------------

##  Hardware
![Test](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Test2.jpg)

![Test](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Test3.jpg)

![Test](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Test1.jpg)

### 1. Choose Camera Lens

The camera of detection of the same area, ‘Camera2’ is angle of view 150°need height is 'h2', and the ‘Camera1’is angle of view 90°need height is 'h1', in order to narrow the project size and choose the lens of angle of view 150°.
![Choose Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/LARGE.jpg)

![Choose Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/different.jpg)
### 2. Calibration the Camera Lens

The picture taken by use the angle of view 150 camera is a distortion, and in order to Calibration the distortion of the picture using Matlab camera Calibration tool ‘Calibration Toolbox Camera’.

![Calibration the Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Calibration%20.jpg)
### 3. The Camera Calibration Toolbox

Although "the camera calibration toolbox" as long as 3 different angles to shoot photos can be a good camera calibration, but in this design used 9 different angles to shoot photos of the camera calibration the calibration parameters, the parameters are applied to opencv correction algorithm in real-time operation can be real-time correction of distortion of the picture.

The following link is the algorithm paper and ‘camera calibration toolbox’ guide

[http://www.vision.caltech.edu/bouguetj/calib_doc/](http://www.vision.caltech.edu/bouguetj/calib_doc/)

[http://research.microsoft.com/en-us/um/people/zhang/Calib/](http://research.microsoft.com/en-us/um/people/zhang/Calib/)

![Calibration the Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Calibration%20Toolbox1.jpg)

![Calibration the Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Calibration%20Toolbox2.jpg)
![Calibration the Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Calibration%20Toolbox3.jpg)
![Calibration the Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Calibration%20Toolbox4.jpg)
![Calibration the Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Calibration%20Toolbox5.jpg)
![Calibration the Camera Lens](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Calibration%20Toolbox6.jpg)

### 4. Lens Process
 
 The PC camera is visible and infrared detection are not allowed to enter because the infrared light in practice may cause the whole picture of the color fidelity, PC camera in the design of the product design manufacturers often use infrared filters in the infrared light environment, in this project need to detect the infrared signal is so PC camera behind add a layer of infrared filter, this filter can filter out visible light infrared light and visible light are only allowed to enter, because of the use of this system is the 980nm infrared laser, so the permeability of 980nm filter, this filter can filter the 980nm light for more than 980nm light has good permeability.
 
![Lens Process](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Lens%20Process1.jpg)

### 5. Why Choose Linear Laser

Select the 980nm infrared linear laser as the source of the signal detection, the linear laser can cover all the keyboard range.

![Why Choose Linear Laser](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Why%20Choose%20Linear%20Laser1.jpg)

![Why Choose Linear Laser](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Why%20Choose%20Linear%20Laser2.jpg)

------------------

### Software
### 1. Programming and Algorithm

Use cvCaptureFromCAM () function and cvQueryFrame () function to get the camera image，Using the cvCvtColor () function binarization the image,Use findContours () function to find the object contour, drawContours () function to draw the object contour, boundingRect () function to draw the object contour of the rectangle
![Programming and Algorithm](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/Algorithm1.jpg)

### 2. Frist you should install Visual Studio 2010 and  download OpenCV library (PS:my project use OpenCV 2.4.6 you can use 2.4.6+).
![Image text](http://note.youdao.com/yws/api/personal/file/48650AC8916F4270866C04A7107A2974?method=download&shareKey=994308b356db4f93fe13a4bf707b0ece)

### 3. you must add the ‘OpenCV library’ to Visual Studio project    
[How to build applications with OpenCV inside the Microsoft Visual Studio](http://docs.opencv.org/2.4/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html)


### 4. If you don't want to compile the code you can run "\OpenCV laser projection virtual keyboard\OpenCV Test\Debug\OpenCV Test.exe" the "OpenCV Test.exe" is my project compiled application ,if you run the application the PC must Plug in the USB camera to run the application again (PS: You must add all "OpenCV library.dll" ).
![Image text](http://note.youdao.com/yws/api/personal/file/BDDA461324CB430CB6068FA210531B7F?method=download&shareKey=a06b4a651447161ccc31922610e47f38)


if you not add all "OpenCV library.dll"  the application will not run successful.
![Image text](http://note.youdao.com/yws/api/personal/file/BD80FB85EE6149B6B86594BC388350AC?method=download&shareKey=3b51a68c948a1ddfda2edb4b49076faf)


![Image text](https://github.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/blob/master/IMG_20140102_202742.jpg?raw=true)
![Image text](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/imgwork.bmp)
![Image text](https://raw.githubusercontent.com/hxy513696765/OpenCV-laser-projection-virtual-keyboard/master/img4.bmp)

## good luck ! 
