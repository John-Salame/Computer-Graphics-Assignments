# Final Project
John Salame
CSCI 5229
Due Tues, 12/6/22

## Potential Additional Work
I have worked for 40-50 hours in the span of a week. In the final week before the project is due, I could probably work 20-30 more hours at most. That would be enough for possibly one or two new objects and one or two new effects. I could probably create three new things at most. I'm thinking of either making a spotlight (which could be tricky since everything is per-pixel lighting) that is on a lamp. I am also thinking of making fog and possibly a blizzard using a white rectangle in front of the face with most of the pixels discarded and maybe some random variation in intensity.

## Normal Map Generator
I created a tool, createNormalMap.py, which creates OK to decent normal maps based on the textures I've created. So far it only creates variations in the tangent part of the normal vector (red). The bitangent (green) is always 128 (which means there is 0 bitangent component to the generated normal vector). I could extend my Python script to create the variations in the bitangent, but that would be a lot of headache adapting my red script to work for the y dimension and the green color.

## Normal Maps
The main feature of the final project is normal maps.
According to my README, I spent anywhere from 32 to 42 hours getting my normal maps to work. The normal maps are a bit weird. It seems to be a moving illusion that changes from bumps to pits depending on which side the light is on. The shadows are always on the correct side, so I think it was just imbalance in the normals my createNormalMap.py generated. The normal map for the candy cane / wooden bench is particularly awful, which is strange because it worked very well when I put it on the ground in testing.

Interestingly, the weird illusion works very well for the snow because it gives the feel that the snow is shifting as it heats up during the day. That makes sense for melting snow.

## Textures
I created some textures in GIMP. I have a few textures for snow to try out, which are all slightly different. I have three snow textures (two are in use), a grass texture which I cropped out of a texture I found (but forgot to cite the source and no longer remember it). I also have a "scratches" sort of texture that I use on the candy canes and the wooden bench seat. 
Press T to toggle textures.

## Day and Night
Scene 0 features a day and night cycle. Let it run on its own, or press C or O (the letter) to control the sun/moon.

## Scenes
I have 4 scenes. Scene 0 is my main scene with lots of things in it. Scenes 1 through 3 have individual objects with a light orbiting them.  
Change the scene by holding CTRL and using the arrow keys. Press M to change projections (you can even walk around in first-person if you press M twice). Please read the controls for other functions, such as pausing the light or moving the light.

## Controls
* Press P to pause the light's motion and look around with the arrow keys (UP/DOWN/LEFT/RIGHT). Please note that "Pause" and "Control Light" are mutually exclusive; enabling one will disable the other.
* Press C or 'o' to enter "Control Light" mode. In this mode, the light will pause and you can move the light with the arrow keys (UP/DOWN/LEFT/RIGHT).
* Press CTRL + Arrow Keys to change the scene.
* Press L to enable or disable lighting. (does not work for objects with shaders)
* Press T to toggle textures. (does not work for objects with shaders)
* Press X to toggle axes.
* Press M (M for mode) to change from Orthogonal to Perspective to First Person.
* Press 0 (zero) to reset the scene.
* First-Person Perspective: Move with WASD (W=forward, A=left, S=down, D=right).
* First-Person Perspective: Turn the camera with the arrow keys.
* Overhead Orthogonal or Overhead Perspective: Spin the scene with the arrow keys.

## Time taken
According to my README, I spent anywhere from 32 to 42 hours getting my normal maps to work. As for added objects, I have only added a bench so far, which took me 7.5 hours. In total, I have spent 40 to 50 hours on the project.

The bench is awful since I only focused on the wooden seat in those 7.5 hours. I want to improve the look of the bench and make a fancier design. I spend too much time testing and trying to get the look right. The wooden look was somewhat difficult to get right. The normal map I use on it is also lackluster. The snow works much better.
