Simple Spectrophotometer
==============

From [Wikipedia](http://en.wikipedia.org/wiki/Spectrophotometry):

*A spectrophotometer is commonly used for the measurement of transmittance or
reflectance of solutions, transparent or opaque solids, such as polished glass,
or gases.*

This is a very simple fixed wavelength spectrophotometer for 1.5 and 2.0 ml
centrifuge tubes that I've designed to aid quantifying algae growth from
different experimental cultures by way of
[fluid turbidity](http://en.wikipedia.org/wiki/Turbidity).

The STL files for its physical components, as well as the Arduino firmware, are
released in the hope that others might find this device useful.

Principle of Operation
==============

Two white LEDs are positioned exactly opposite each other on either side of
the sample tube, one acting as a reference light source, and the other acting
as a ["Mimms Effect"](http://en.wikipedia.org/wiki/Forrest_Mims#Using_LEDs_as_narrow_band_light_sensors) photodiode.

The Arduino firmware reads the value off of the light sensor and compares that
to calibrated values for pure water to determine the sample's opacity compared
to pure water.

Further Work
==============

- Future improvements could include a tri-color LED to measure light
transmission at different wavelengths, bringing the device closer to a proper
spectrometer.

- More experiments are needed with different LEDs as the light sensor, other
colors are reported to work better.

Requirements 
==============

To compile this firmware, you'll need the following Arduino libraries:

#### Bounce2

http://github.com/thomasfredericks/Bounce-Arduino-Wiring/tree/master/Bounce2

#### LiquidCrystal_I2C

http://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home

License
==============

The software is distributed under a BSD 3 clause license in the hope that it
will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

For details, see the file "LICENSE" in the source directory.

The physical objects are released under the Creative Commons BY-SA license.

For details, see: http://creativecommons.org/licenses/by-sa/3.0/
