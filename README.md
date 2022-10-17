# Homework 5: Lighting
John Salame
CSCI 5229
Due 10/6/22

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

