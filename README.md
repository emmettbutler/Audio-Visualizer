Audio Visualizer
================

![image](http://24.media.tumblr.com/25c39888ac15999a6c81e9b5f7155f0e/tumblr_mfeitgu6UI1qbeadxo1_1280.gif)

This project provides realtime 3d visualization of WAV audio streams. The visualizations created are mainly abstract and can be considered an emulation of the iTunes (or, more accurately, WMP) visualizer. It uses OpenGL, freeglut, portaudio and libsndfile to create these visuals in real time.

Invoked from the command line, the visualizer takes the name of an audio file and a number of flags specifying graphical behaviors. These can range from the basic flat waveform to a colorful, dynamic shape of constantly-changing data.

The main challenge of the project (beyond learning a book's worth of 3D graphics theory and practice) was to properly synchronize the audio streaming with the graphics rendering loop. This is accomplished using a shared buffer contianing a number of renderable "screens" worth of audio waveform data. The audio streamer acts as a server that fills free slots in the buffer, and the graphics rendering client pulls data from the filled slots. Since the audio runs at a much faster rate than the graphics, the audio ends up waiting around a bit. This is ok, since it has plenty of heavy number-crunching capacity as visualizations grow more advanced.
