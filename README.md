Stereo to binaural plugin demo that receives camera head-tracking data via UDP.

The left and right inputs are convolved with HRIR's using [libmysofa](https://github.com/hoene/libmysofa)
to become 'virtual loudspeakers' that can be moved around the head.
I implemented a time-varying UDP convolver to cross-fade (in time-domain) between IR's.

The binaural renderer was adapted from [Sonority](https://github.com/MicahStrange/sonority), and the convolver
was built the UDP convolver in [Zones Convolver](https://github.com/zones-convolution/zones_convolver), and its utility
classes.

The head tracking is from [EACHeadtracker](https://github.com/eac-ufsm/webcam-headtracker). The
python script is run from the terminal and sends Yaw and Pitch data using UDP.

Thanks to [Pamplejuice](https://github.com/sudara/pamplejuce) for the plugin template.

### Improvements

There's quite a few improvements needed for this to be viable beyond a demo.

* integrating the head tracking into the plugin. I don't know how feasible
  camera access inside a DAW is but potentially the user could select it inside the plugin
  which gets terminal to run the script separately.
* At the moment the HRTF isn't very convincing but some filtering and some reverb control to
  give the virtual speakers depth would help.
* The convolver could be improved with frequency domain cross-fading and not convolving unused
  filter partitions.
* Some nice looking UI and maybe a way of visualising the movement caused by the
  head tracking.