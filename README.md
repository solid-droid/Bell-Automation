# Bell-Automation
Esp8266 WiFi- electric bell controll system.
The bell automation system is a wireless bell control system. It features Wi-Fi,       time & date storage and multiple secured user access capabilities, all programmed inside the embedded hardware. It also features a manual real time ringing of the bell wirelessly and dedicated time and date slots which can store up to 200 distinct entries                    (more than 28 entries a day) wirelessly. 
	The UI is basically a website from the local web server running in the microcontroller. This website is accessed using the dedicated IP address given to the microcontroller.        The UI welcomes the user with a login screen. The Admin Control Panel is visible after the secure login is made. In the Admin Control Panel the user is given permission to manipulate the following 4 features: 
i.	Manual real time bell ring control. 
ii.	Ring duration control.
iii.	Time and Date slot updating fields for automatic ringing.
iv.	Time and Date slot removal fields for removing each updated slots.
The system also includes a led indicator which will be lit continuously if everything is working fine. If the led starts blinking, this indicates either the Wi-Fi connection is lost or the system lost its current reference time.

Hardware
ii.a Layout
                                                                              


ii.b Components
SL
NO.	  Type	 Serial	Price 
1	Micro-controller	ESP8266-12E Node MCU LoLin	415 INR x 1
2	Regulator (12V DC)	7812	40   INR  x1
3	Transformer + Rectifier	Center tapped (240v-12v)	140 INR  x1
4	Relay	JQC-3FC(T73) 12VDC (240V ~ 7A)	25   INR  x1
5	Dot Board	14 x 5cm	 50  INR  x1
									TOTAL       670 INR
  	Microcontroller power consumption: 70mA – 320mA @3.3v.




Software
iii.a UI/UX

The index page UI consist of 2 input fields for username and password. The correct values are stored inside the micro-controller which cannot be changed afterwards without the source code.
The form sends the data to the local web server using the POST method hence making the values not visible to the outside world. If the username and password matches, the Admin Control Panel is displayed. 




The Admin Control Panel consists of 5 input fields and 2 notification spaces. The ‘Time’ and ‘Day’ fields are same for both update and delete functions. The duration is              4 seconds by default, which is the same for all the time slots and the manual function. If the ‘Manual’ is checked, whenever the submit button is pressed the bell will ring for the given duration. Since the system runs on Software Real Time Clock, whenever the power break happens or the microcontroller resets, the system should be provided with the current time. This can be done by simply pressing the submit button.

The Current Time Set displays all the stored time slots. This information is stored in the EEPROM of the microcontroller; hence it is not affected by microcontroller resets.
iii.b Embedded
	The Update and Delete functions uses the EEPROM functionality of the microcontroller. Whenever the submit button is pressed the current time and date are always passed to the server. Whenever an update submission is made, if and only if the ‘Time’ and ‘Day’ fields are filled completely, the webpage uses the POST method to submit the time and date into the ring slot list. The ring slot list is basically an EEPROM array programmed inside the microcontroller. The registered sets are displayed to the user via the ‘Current Time Set’ section using AJAX script hence it’s a live list. The list can have up to 200 entries which is more than 28 entries per day.
	The delete function also uses the ‘Time’ and ‘Day’ fields. Whenever a delete submission is made, if the ‘Time’ and ‘Day’ fields are filled completely, the webpage uses the POST method to submit the time and date to the server. If such a slot exists in the ‘Current Time Set’ the microcontroller erases the entry and displays the new list.
	Whenever a submission is made with a checked ‘Manual’ the bell rings. The duration field is by default 4 seconds which can be varied from 1sec to 99sec (Integers only). This duration enables the same ring duration for all manual and stored ring sets. The duration info is not stored in the EEPROM and hence should be updated whenever the microcontroller resets else the default value will be used.
	The indicator led can have 3 states:
1.	OFF state
Causes: 1. No power supply.
	         	    2. Microcontroller Malfunctioning.
	2. Blinking State
		Causes: 1. No Wi-Fi connection.
				(WiFi connection is not a necessary for the proper functioning of the system)
				(But it is better to keep the system connected.)
		               2. System lost the current Time and Day.
           (Click the Submit button from the Admin Control Panel to solve this)
2.	ON State
     Cause:   1. The system is working perfectly.




iv. Setting Up
	The product consists of 2 pairs of power sockets. Each socket has 2 screws sockets, one for phase and neutral. The first pair is used to power the product while the second pair is used for connecting the electric bell. The neutral is internaly shorted for both the sockets.
 An ON/OFF switch is present on the other side which can be used to switch ON or OFF the device manually.
Whenever the device turns ON, the indicator led starts blinking .The system will automatically connect to the defined access point if available. Use a web browser on the connected computer to access the device control panel using the static IP address. Click submit button on the admin panel to set the current time and day. This should make the blinking LED to stay lit continuously indicating the device is working perfectly. 
