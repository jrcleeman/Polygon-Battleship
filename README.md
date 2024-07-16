# Polygon Battleship
## Basic Game Play
Use W,A,S,D to navigate your Polygon (red outline black fill) <br/>
Use the mouse and left click to fire bullets at a desired position <br/>
Bullets have a finite life span <br/>

## Enemies
Enemies will appear at fixed intervals with random number of sides, outline color, and fill color <br/>
Enemies will bounce off walls upon collision <br/>
Enemies will explore into a numer of smaller versions of themselves depending on the number of sides they possess <br/>
Smaller enemies have a finite lifespan <br/>
Collision with regular or small enemies will result in Polygon respawn at the center of the game <br/>
Score points are assigned based on number sides and size of enemy destroyed <br/>

![bandicam2024-07-0618-18-39-262-ezgif com-crop](https://github.com/user-attachments/assets/487c2037-d923-4c22-96ba-5edeb76c5407)

## Special Weapon
If surrounded by enemies fire your special weapon using a right click! <br/>
The special weapon has a fixed cooldown time <br/>
![bandicam2024-07-1612-00-54-277-ezgif com-crop](https://github.com/user-attachments/assets/31963798-6079-41f7-bb87-6fad151c2c5f)

## ImGui
The ImGui drop down in game can be used to manipulate various game parameters <br/>

## Details
This project is created using SFML and a Entity Component System architecture taught by Dave Curchill <br/>
