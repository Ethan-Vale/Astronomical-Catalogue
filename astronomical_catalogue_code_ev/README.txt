Heres a quick guide on how to use the code:

HOW TO MANUAL INPUT
all objects must start with their object type:
galaxy, star, planet, moon.

Following this:

galaxy,hubble type,name,RA,DEC,dist,magnitude,redshift,helio-radial velocity
star,host galaxy,name,RA,DEC,distance,magnitude,evolutionary stage,mass,temp
planet,host star,name,mass,period,semi-major axis,eccentricity
moon,host planet,name,mass,period,diameter

RA and DEC are read in as J2000 format (so RA: hh mm ss and DEC: degrees mm ss)

TEST DATA 
Alongside the code files are two sets of test data:

mixed_test.csv contains data of various object types with some fake errors.
This is used to test full functionaility of the code besides the sorting function, so go to town on this file

gal_test.csv contains just galaxy data
This can be used to demonstrate the sorting function


Okay thats about it please mark fairly haha.