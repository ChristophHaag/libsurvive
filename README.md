# libsurvive [![Build Status](https://travis-ci.com/cntools/libsurvive.svg?branch=master)](https://travis-ci.com/cntools/libsurvive)


![Logo](https://cloud.githubusercontent.com/assets/2748168/24084003/9095c98a-0cb8-11e7-88a3-575f9f4c7bb4.png)

An Open-Source tool for working with lighthouse-based tracking data, including support for the HTC Vive, Vive Pro and Valve Index.

Most of the development is discussed on Discord.  Join the chat and discussion here: https://discordapp.com/invite/7QbCAGS

## Livestream collection

If you are an individual who bothers going back and watching some of these, PLEASE PLEASE PLEASE add a note to https://github.com/cnlohr/libsurvive/issues/66 whenever something interesting happens.  I want to make a highlight reel, but I don't want to sit through all the videos.

| Note                                   | Youtube URL                                 | Run time |
| -------------------------------------- | ------------------------------------------- | -------- |
| First livestream                       | https://www.youtube.com/watch?v=sv_AVI9kHN4 | 5:01:25  |
| Second livestream                      | https://www.youtube.com/watch?v=gFyEbGQ88s4 | 4:03:26  |
| Summary of first and second livestream | https://www.youtube.com/watch?v=oHJkpNakswM | 23:00   |
| Third livestream                       | https://www.youtube.com/watch?v=RExji5EtSzE | 4:11:16 |
| Fourth livestream                      | https://www.youtube.com/watch?v=fces1O7kWGY | 4:50:33 |
| Fifth livestream                       | https://www.youtube.com/watch?v=hHt3twW5_fI | 3:13:38 |
| Sixth livestream                       | https://www.youtube.com/watch?v=JsfkNRFkFM4 | 3:44:49 |
| Seventh livestream                     | https://www.youtube.com/watch?v=EKSHvO3QSWY | 1:17:21 |
| Eighth livestream                      | https://www.youtube.com/watch?v=nSbEltdH9vM | 6:06:36 |
| Ninth livestream                       | https://www.youtube.com/watch?v=60sGTd8T-KY | 3:28:44 |
| Ninth B livestream                     | https://www.youtube.com/watch?v=IIYj1Ig_gz8 | 2:25:33 |
| May 2017 Summary                       | https://www.youtube.com/watch?v=dVXpHKktbzM | 5:58    |
| Tenth livestream                       | https://www.youtube.com/watch?v=8hqviGMlU7I | 3:30:46 |
| Vive 2.0 Lighthouse Reverse Engineering Pt 1. | https://www.youtube.com/watch?v=FtXkjPcmENA | 2:39:43 |
| Vive 2.0 Lighthouse Reverse Engineering Pt 2. | https://www.youtube.com/watch?v=8YPdmgOXnPM | 1:54:23 |
| Tearing down a Vive Lighthouse 2.0.    | https://www.youtube.com/watch?v=JP5maGaumYo | 2:35:49 |
| First shot at trying to decode Lighthouse 2.0 Data |https://www.youtube.com/watch?v=Ij1oqvcllUs | 2:28:21 |

Notes from second livestream trying to reverse engineer the watchman protocol: https://gist.github.com/cnlohr/581c433f36f4249f8bbc9c2b6450ef0e

Please see the issues for what help needs to be done now!

## Extra resources

HackADay article and video with Dr. Yates on how they made the Vive a thing. http://hackaday.com/2016/12/21/alan-yates-why-valves-lighthouse-cant-work/

Excellent article written by Oliver Kreylos showing just how amazing the lighthouse system really is.  Likely the best/most detailed review on the performance *and* accuracy of the lighthouse system. http://doc-ok.org/?p=1478  


## Nomenclature

* WRT = With Respect To
* PoV / POV = Point of View (typically WRT to a LH, sometimes (though rarely) a sensor)
* LH = Lighthouse = Base Station = A device that produces a 1.8 MHz modulated sync pulse in IR and then sweeps the scene with laser planes.
* Sync Pulse = A pulse of modulated IR data sent from a ligthhouse, typically by the floodlight aspect of a lighthouse.
* Sweep Pulse = The evenlope created by a laser sweeping over a light sensor.
* OOTX = Omnidirectional Optical Transmitter = Data encoded in the sync pulses of the LHs.
* HMD = Headset = Main sensor receiver with a visual display for a human.
* WM = Watchman = Controller = The HTC Vive controller.
* TR = Tracker = Official HTC Tracker.
* LightcapElement = A single pulse of light, including a timestamp, source sensor and length of pulse.
* Disambiguator = System that accepts lightcap elements and pulls out OOTX data and relative sweep times of sweep pulses.
* Poser = Device to convert series of angles from a LH's PoV

## Getting things working

There are two things you should consider doing to your system before running libsurvive.

(1) Install the udev rules:  ```cp useful_files/81-vive.rules to /etc/udev/rules.d/``` and reboot.
(2) If you are running on an NVIDIA Card, you will need to AllowHMD to true.  Add the following line to your /etc/X11/xorg.conf device section:  ```Option "AllowHMD" "yes"```

## Introduction
High-performance HTC Vive Library

I say "high-performance" really this project is based tightly off of OSVR-Vive-Libre, but, specifically is an attempt to:

1. Minimize external libraries.  Actual reason for starting this: Downloading all of the libraries needed for OSVR-Vive-Libre maxed out my data plan.
2. Put it under an open-source instead of a force-source license.  (GPL to MIT/X11)
3. Write it in C.
4. Avoid extra layers where convenient.
5. (long shot) Make the vive viable for use with Intel Integrated Graphics systems. [It works with HD4000 using DisplayPort. See "Intel Integrated Graphics" section below.]

Will ~~I~~ we succeed?  Probably not.  ~~Definitely going to try!~~ Though it's looking like we might.


## External dependencies

* libUSB (Linux) or hidapi (Win, OSX; included in redist)
* pthread
* libX11 (Linux) or Native (win32) or OpenGL (OSX)
* zlib (Linux) or puff.c (win32, included in redist)
* Optionally OpenGL.
On Debian, ```sudo apt-get install build-essential zlib1g-dev libx11-dev libusb-1.0-0-dev freeglut3-dev``` should be sufficient.

Temporarily needed packages: liblapack and libopenblas
On Debian, ```sudo apt-get install liblapacke-dev libopenblas-dev libatlas-base-dev```

## Architecture

<TABLE><TR><TH>Description</TH><TH>Diagram</TH>
</TR><TR>
<TD WIDTH=50%>

### Layout

In the src/ folder you'll find most of the internal code that is part of libsurvive.  The redist/ folder contains code that libsurvive uses that was copied from other projects.  Libsurvive links to other libraries, but very few.  You'll find that most of the functionality lies within libsurvive or in the redist folder.  For the user-facing headers you can find them in the include/ folder.

### Logical Data Flow

There are device drivers, such as survive_vive.c which connect to physical devices, via libUSB, hidapi or another method and collect raw IMU and lightcap data.  Lightcap data is specically a sensor ID, light pulse length (in ticks) and the time of the light pulse (in ticks).  The driver also must provide locations of the sensors to populate the SurviveObject structures of whatever sensors that driver is responsible for.

Once this data is collected, the light pulses are disambiguated (see survive_data.c) into OOTX sync pulses (id -1, -2 depending on lighthouse) as well as sweep pulses which provide the time of a sweep pulse passing the sensor.  This is passed off to "lightproc."  The default behavior for lightproc can be found in survive.c.  The user may override this, however, if they have interest in the raw pulse information for whatever reason.  The default behavior for lightproc determines the time delta between the sync pulse and the sweep pulse time and derives angle.  The derivation for the angle is simply calculated by the time difference between the sync pulse and the sweep pulse.   It then calls "angleproc"  (not implemented yet: Using OOTX data from lighthouses to correct and tweak angles)

Angleproc may also be overridden by the user for similar purposes to for "angleproc" which passes its information off to a calibrator (if running) as well as to whatever posers are enabled.  The posers will take this data and determine position from it.

</TD>
<TD WIDTH=50%><img src=https://raw.githubusercontent.com/cnlohr/libsurvive/master/useful_files/FunctionalSystem.png width=400></TD>
</TR>
</TABLE>

## Lists of components

Component Type | Component | Description | Authors
--- | --- | --- | ---
Poser | [poser_charlesslow.c](src/poser_charlesslow.c) | A very slow, but exhaustive poser system. Calibration only. | [@cnlohr](https://github.com/cnlohr)
Poser | [poser_daveortho.c](src/poser_daveortho.c) | A very fast system using orthograpic view and affine transformations. Calibration only (for now) | [@ultramn](https://github.com/ultramn)
Poser | [poser_dummy.c](src/poser_dummy.c) | Template for posers | [@cnlohr](https://github.com/cnlohr)
Poser | [poser_octavioradii.c](src/poser_octavioradii.c) | A potentially very fast poser that works by finding the best fit of the distances from the lighthouse to each sensor that matches the known distances between sensors, given the known angles of a lighthouse sweep.  Incomplete- distances appear to be found correctly, but more work needed to turn this into a pose. Based on [this python code](https://github.com/octavio2895/lh_tools). | [@mwturvey](https://github.com/mwturvey) and [@octavio2895](https://github.com/octavio2895)
Poser | [poser_turveytori.c](src/poser_turveytori.c) | A moderately fast, fairly high precision poser that works by determine the angle at the lighthouse between many sets of two sensors.  Using the inscirbed angle theorom, each set defines a torus of possible locations of the lighthouse.  Multiple sets define multiple tori, and this poser finds most likely location of the lighthouse using least-squares distance.   Best suited for calibration, but is can be used for real-time tracking on a powerful system.  | [@mwturvey](https://github.com/mwturvey)
Poser | [poser_epnp.c](src/poser_epnp.c) | Reasonably fast and accurate calibration and tracker that uses the [EPNP algorithm](https://en.wikipedia.org/wiki/Perspective-n-Point#EPnP) to solve the perspective and points problem. Suitable for fast tracking, but does best with >5-6 sensor readings. | [@jdavidberger](https://github.com/jdavidberger)
Poser | [poser_barycentric_svd.c](src/poser_barycentric_svd.c) | Generalizes the barycentric / svd approach used with EPNP; but allows for different plane equations to support LH2 | [@jdavidberger](https://github.com/jdavidberger)
Poser | [poser_sba.c](src/poser_sba.c) | Reasonably fast and accurate calibration and tracker but is dependent on a 'seed' poser to give it an initial estimate. This then performs [bundle adjustment](https://en.wikipedia.org/wiki/Bundle_adjustment) to minimize reprojection error given both ligthhouse readings. This has the benefit of greatly increasing accuracy by incorporating all the light data that is available. Set 'SBASeedPoser' config option to specify the seed poser; default is EPNP. | [@jdavidberger](https://github.com/jdavidberger)
Poser | [poser_mpfit.c](src/poser_mpfit.c) (default) | Performs Levenberg-Marquardt using [MPFIT](https://www.physics.wisc.edu/~craigm/idl/cmpfit.html). Since SBA does basically the same thing, this poser gets nearly identical results to SBA. Overall it is a tad slower than SBA since SBA uses optimized lapack functions to solve Ax=b, but MPFIT has the distinction of not needing lapack at all since it's Ax=b solver is a minimal internal version. It also requires a seed poser. | [@jdavidberger](https://github.com/jdavidberger)
Disambiguator | [survive_charlesbiguator.c](src/survive_charlesbiguator.c) | The old disambiguator - very fast, but slightly buggy. | [@cnlohr](https://github.com/cnlohr)
Disambiguator | [survive_turveybiguator.c](src/survive_turveybiguator.c) | More complicated but much more robust disambiguator | [@mwturvey](https://github.com/mwturvey)
Disambiguator | [survive_statebased_disambiguator.c](src/survive_statebased_disambiguator.c) (default)  | A fast disambiguator that was times the state shifts between pulses. Experimental. Made to allow tracking very close to the lighthouse | [@jdavidberger](https://github.com/jdavidberger)
Dismabiguator | superceded disambiguator | A more sophisticated disambiguator, development abandoned.  Removed from tree. |  [@jpicht](https://github.com/jpicht)
Driver | [survive_vive.c](src/survive_vive.c) | Driver for HTC Vive HMD, Watchmen (wired+wireless) and Tracker | [@cnlohr](https://github.com/cnlohr) and [@mwturvey](https://github.com/mwturvey)
OOTX Decoder | [ootx_decoder.c](src/ootx_decoder.c) | The system that takes the pulse-codes from the sync pulses from the lighthouses and get [OOTX Data](https://github.com/nairol/LighthouseRedox/blob/master/docs/Light%20Emissions.md) | [@axlecrusher](https://github.com/axlecrusher)

## Component Pluggability Matrix

Component Type | Pluggability method
--- | ---
Driver | Dynamically loadable runtime, can co-exist with other drivers.
Poser | Selectable by configuration at runtime
Disambiguator | Selectable by configuration at runtime
OOTX Decoder | Not Pluggable



## Intel Integrated Graphics

The limiting factor for Vive viability on a given computer is the maximum available pixel clock frequency, and frequency limitations of the HDMI port, and HDMI and DisplayPort video cables. DisplayPort can support higher frequencies than HDMI, on Ivy Bridge HD4000 graphics. In fact, the Vive works with HD4000 graphics using DisplayPort, with native EDID resolution (2160x1200@90Hz).

To support the Vive on HDMI, you either need a newer version of HDMI, or you need to define a custom resolution that respects pixel clock and video port limits, and is also accepted and displayed by the Vive. So far, we have not had success using custom resolutions on linux or on Windows. Windows imposes additional limitations in the form of restriction of WHQL certified drivers forbidden from using custom display resolutions (only allowing those defined by EDID in the monitor). Intel has released uncertified beta drivers for Haswell and newer processors, which should be able to support custom resolutions for the Vive (untested at this time).

# Getting Started

lapacke and OpenCV are both supported as math backends, though it is recommended to install lapacke to avoid compilation issues with recent OpenCV versions.

```
sudo apt-get install liblapacke-dev libopenblas-dev libatlas-base-dev
```

```
git clone https://github.com/cnlohr/libsurvive.git && cd libsurvive
make

# Run any libsurvive application to create calibration files for connected HMDs, Trackers.
./survive-cli
```

## General Information

Libsurvive automatically runs lighthouse calibration on starting up as needed and caches the calibration in the current working directory. The lighthouse calibration is stored as `config.json`. Additionally, each device contains a fixed configuration, which is dumped to a file `<devicename>_config.json`.

All libsurvive tools support libsurvive specific command line arguments. Third party applications using libsurvive may choose to support this, but are free not to.

See for example `survive-cli --help` for available arguments.

### LH1 Calibration / Setup

On the first start, any libsurvive application will automatically calibrate. This involves determining the positions of the lighthouse basestations relative to the tracked device, including the positions of the basestations relative to each other. When moving a basestation, you should probably re-run calibration.

Calibration can be forced to run again with the argument `--force-calibrate`, or by simply deleting the config.json file.

`--requiredtrackersforcal T20` can be used to calibrate using the sensors of a specific device, here the `T20` device.

**The HMD should not be moved while calibrating!** The tracked device should not be too near the lighthouse basestation, try to get at least 30-50 cm distance between them.

Calibration can take a few seconds, but it should not take much longer than 10-20 seconds. If it takes much longer, try to move the device so that more sensors are hit by the basestations and make sure there are no reflective surfaces. If nothing helps, libsurvive may fail to detect that your device is not being moved, or a bug prevents calibration from running.

lighthouse v1 calibration is successful after going through 4 stages:
```
Info: Stage 2 good - continuing. 32 1 0
Info: Stage 2 good - continuing. 32 1 1
Info: Stage 2 good - continuing. 32 1 2
Info: Stage 2 good - continuing. 32 1 3
Info: Stage 2 good - continuing. 32 1 4
Info: Stage 2 moving to stage 3. 32 1 5
Lighthouse Pose: [0][ 0.28407975, 0.93606335,-0.37406892] [ 0.05594964,-0.33792987, 0.93887696, 0.03439615]
Info: Stage 4 succeeded.
```
[For reference, here is an older recording of how a properly running calibration looks like](https://haagch.frickel.club/Peek%202018-02-21%2023-23.webm).

### LH2 Calibration / Setup

Calibration and setup for lighthouse 2 setups has been streamlined somewhat. On initial startup, it will take ~10 seconds
from seeing a lighthouse to reading its ID and calibration data. After that, it waits for the device to not move and then
automatically solves for its position.

When it sees new lighthouse devices; it will solve for their position in the global system whenever it sees no device
movement based on the IMU.

lighthouse v2 calibration is successful when libsurvive found Positions of the basestations:
```
Info: Attempting to solve for 0 with 30 meas
Info: Attempting to solve for 1 with 21 meas
Info: Solved for 0 with error of 0.005446/0.000000
Info: Solved for 1 with error of 0.005446/0.000000
Info: Using LH 0 (d99e7eac) as reference lighthouse
Info: Position found for LH 0(d99e7eac)
Info: Position found for LH 1(fe0398ef)
```

## Using libsurvive in your own application

Libsurvive offers a low level API as well as a higher level application API.

Example code for the application interface can be found in [api_example.c](https://github.com/cntools/libsurvive/blob/master/api_example.c). Have a look at the other libsurvive tools a the top level of the repository for example usage of the lower level API.

# Record / Playback

libsurvive has an integrated tool that allows you to record and playback streams from all supported devices. To save off a stream, invoke it as follows:

```
make
./data_recorder --record my_playback_file.rec.gz
```

This gives you a file -- `my_playback_file.rec.gz` -- with all the device configurations and events file you need to replay it.

You can also just let it stream to standard output, but this tends to be a lot of information. 

To actually replay it, put that directory path in the 'playback' configuration value in config.json and run libsurvive as usual. Note that this will purposefully stop the USB devices from loading as to not confuse the library with inconsistent data.

You can also replay it just with command line options:

```
./calibrate --playback my_playback_file.rec.gz
```

Based on the naming convention used, this will gzip the data on the fly. Omit the `.gz` for the raw text format.

## Playback speed

There is also a config variable -- `PlaybackFactor` -- which adjusts the speed at which playback happens. A value of 1 emulates the same time the events file took to create, a value of 0 streams the data in as fast as possible. 

# USBMON

Occasionally, when dealing with new hardware or certain types of bugs that cause an issue in the USB layer, it is necessary to have a raw capture of the USB data seen / sent. The USBMON driver lets you do this.

Currently this driver is only available on linux and you must have libpcap installed -- `sudo apt install libpcap-dev`. You also need the `usbmon` kernel module installed; but many linux flavors come with that built in. 

To start usbmon and prepare it for use for all users, run:

```
sudo modprobe usbmon
sudo setfacl -m u:$USER:r /dev/usbmon* # In sensitive environments, you can run survive-cli with sudo instead.
```

To capture usb data, run:

```
./survive-cli --usbmon-record my-recording.pcap.gz --htcvive <additional options>` 
```

You can run that playback with:
```
./survive-cli --usbmon-playback my-recording.pcap.gz [--playback-factor x] <additional options>` 
```

If you are sending this file for analysis, note that you need the accompanying `*.usbdevs` file with it to be useful. If you follow the `*.pcap.gz` convention, run something like

```
zip logs.zip *.pcap* config.json
```

and post the `logs.zip` to an issue or to discord. 

This driver specifically only captures devices on a white list of VR equipment; but if you don't want to publish raw USB
data to the internet, ask in discord for who to send it to in a private message. 

# Visualization

- Download and install: http://websocketd.com/
- Build the repo
- Run `data_recorder` through `websocketd` like so:

``` websocketd --port=8080 ./data_recorder```

- Navigate to the `/tools/viz/index.html` page in chrome.

Nothing will happen until you connect to that page. When you do, the app lifetime will be bound to that session -- reloading the page will reload data_recorder. 

The arrow keys will move you to the left / right / up / down and the UI response to orbital mouse controls. 

![Visuzliation Screenshot](https://raw.githubusercontent.com/cnlohr/libsurvive/master/useful_files/viz_screenshot.png)

## Notes about coordinate frames.

1) We are using the right-hand rule. 

2) All "poses" should be handled as a SurvivePose, which is effectively a FLT[7], with xyz, wxyz.  The first is a positional offset, and the second is the rotation. These poses are effectively transforms from the object local space to the global coordinate space.

3) Though this is not universal, consider using SurvivePose* for all situations where a true pose used instead of passing around a FLT* or FLT[7].

4) All posers should; given a lighthouse:
 * at 0, 0, 0
 * laying on its back
 * pointed upward
 * receiving a "up" vector from the accelerometer of (0,0,-127)

Given an HMD:
 * pointed FACE DOWN at the lighthouse
 * 1M above the lighthouse

   Will produce a pose of [[0, 0, -1] [0, 1, 0, 0] ].  NOTE: The quaternion is inverted, so rotation axis is arbitrary, point is w is 0.  This is the pose of the OBJECT in WORLD space. For our example, the lighthouse is at 0,0,0 in world space.  In practicality, the lighthouse will be at some other place in the scene.

   The idea is you should be able to take a coordinate local to the HMD and then use ```ApplyPoseToPoint``` to transform that point into world space, by applying the output of the poser.

5) Defining the value of ```pose``` in ```BaseStationData```: This pose will convert something in lighthouse-local space into a position in world space.  When calibrating, if you have an object you want to define as 0,0,0... You will need to take its pose and invert it.

6) Posers will take into account the ```pose``` of various lighthouses, if NOT in calibration and return poses of objects in world space assuming the lighthouses are set up.

7) Objects internally are tracked in IMU space. They are then transformed into 'Head' space, as defined in their config.

General information for LH pose:

![LHPose](https://raw.githubusercontent.com/cnlohr/libsurvive/master/useful_files/lighthouse_pose.jpg)

![TrackerCF](https://raw.githubusercontent.com/cnlohr/libsurvive/master/useful_files/tracker_coordinate_frame.png)

![HMDCF](https://raw.githubusercontent.com/cnlohr/libsurvive/master/useful_files/hmd_coordinate_frame.jpeg)


# FAQ

 * There is an unofficial [OpenHMD/libsurvive fork](https://github.com/ChristophHaag/OpenHMD/tree/libsurvive) that replaces OpenHMD's Vive driver with libsurvive. OpenHMD will not merge this branch as it depends on libsurvive as an external dependency, but it may pave the way for more code sharing.
   * This OpenHMD/libsurvive fork can be plugged into [SteamVR-OpenHMD](https://github.com/ChristophHaag/SteamVR-OpenHMD) which allows SteamVR to use OpenHMD drivers.
   * Godot 3.x has a [native OpenHMD plugin](https://github.com/BastiaanOlij/godot_openhmd) though it needs work for building and running properly and it is still missing motion controller support.

## Addendum and notes

Thanks to Mr. Faul for our logo!
Special thanks to @nairol for an extreme amount of detail in reverse engineering the existing HTC Vive system on his https://github.com/nairol/LighthouseRedox project.
