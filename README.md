# Homework 6: Textures
John Salame
CSCI 5229
Due 10/13/22, extended to Mon, 10/17/22

## Textures
I created some textures in GIMP. I have a few textures for snow to try out, which are all slightly different. Currently, only snow.bmp and snow3.bmp are in use.  
I did not put textures on the main part of the candy cane because it looked awful on top of the glossy look of the candy cane. My idea was to add the illusion of depth, but that is not possible unless the candy cane can lose its shine on the damaged parts or machine cut marks. This is not possible with lighting settings that are uniform across the candy cane like I currently have.  
Press T to toggle textures.

## Day and Night
Scene 0 features a day and night cycle. Let it run on its own, or press C or O to control the sun/moon.

## Scenes
I have 3 scenes. Scene 0 is my main scene with lots of things in it. Scenes 1 and 2 have individual objects with a light orbiting them.  
Change the scene by holding CTRL and using the arrow keys. Press M to change projections (you can even walk around in first-person if you press M twice). Please read the controls for other functions, such as pausing the light or moving the light.

## Controls
* Press P to pause the light's motion and look around with the arrow keys (UP/DOWN/LEFT/RIGHT). Please note that "Pause" and "Control Light" are mutually exclusive; enabling one will disable the other.
* Press C or 'o' to enter "Control Light" mode. In this mode, the light will pause and you can move the light with the arrow keys (UP/DOWN/LEFT/RIGHT).
* Press CTRL + Arrow Keys to change the scene.
* Press L to enable or disable lighting.
* Press X to toggle axes.
* Press M (M for mode) to change from Orthogonal to Perspective to First Person.
* Press 0 (zero) to reset the scene.
* First-Person Perspective: Move with WASD (W=forward, A=left, S=down, D=right).
* First-Person Perspective: Turn the camera with the arrow keys.
* Overhead Orthogonal or Overhead Perspective: Spin the scene with the arrow keys.

## Time taken
I took 17.75 hours to complete the assignment.  
Quite a bit went into debugging my error with lighting rotated objects (it turns out I had an indentation error in my Project function and I never left the Project matrix mode, which somehow caused the light to be at the same coordinates relative to the transform for EVERY vertex I drew instead of taking a global position).  
A lot of time went into fine-tuning the look as well. I think it turned out pretty well. There aren't many objects, but my focus was more on trying out different combinations of ambient, diffuse, and specular lighting.

