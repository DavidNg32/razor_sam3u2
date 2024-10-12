# BOMB DEFUSAL GAME

A bomb defuse game made on an Engenuics development board

## Description

This project implements a bomb defusal game using an Engenuics development board as a final project of University Of Calgary [Embedded In Embedded](https://embeddedinembedded.com/) Mentoring Program. Players must input the randomized sequence generated via the LCD Screen via push buttons as much as possible within a set time. The game is built using a combination of LED, LCD lights, Buttons to create a satisfying and responsive product.

## Showcase
* The program contains two mode.
  * Normal mode provides regular sequence game:
![20241012_004746](https://github.com/user-attachments/assets/09808b28-7279-42e9-b846-49a637b015a6)

  * Blind mode provides a harder experience by removing the sequence after you press a button making it so that you have to remember the sequence before attempting:
![20241012_005757](https://github.com/user-attachments/assets/9770748f-0a52-4c44-a8d4-ae03867b762a)

* Upon starting the game:
  ![20241012_004959](https://github.com/user-attachments/assets/091cf824-72aa-4076-9b75-978fffcd650d)

  Press the corresponding button as the displayed sequence with a limited timer on the top left

* Upon pressing one wrong button or the timer ran out:
  ![20241012_005116](https://github.com/user-attachments/assets/a6906fed-04be-4cd2-86b8-ff6b4732caa0)

  The score is displayed for bragging rights.



## Getting Started

### Dependencies

* IAR Embedded Workbench
* Eugenuics Development Board

### Installing
* Clone the repositories on github
```
git clone https://github.com/DavidNg32/EiE_DefuseGame.git
```
* Open the project file from IAR Embedded Workbench
* Ensure that you have J-Link driver downloaded and Eugenics Board connected to your computer.


### Executing program
* Press build button on the IAR interface.

## License

This project is licensed under the [MIT] License - see the LICENSE.md file for details

## Acknowledgments
* Engenuics for providing the development hardware and codes
* Matthew for being a great team leader.


