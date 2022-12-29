<h1 align="center">IoT Study and Sleep Monitoring</h1>
<h2 class="Overview">Overview</h2>
<p>This is a project for mid semester exam. This project is about a device that can be use as a bedtime alarm. The user first must set the bedtime alarm with an infrared remote and it will be saved in the device. The alarm will rang at the bedtime, and the user will be able to stop it. It is assumed that the time the user stops the alarm is the time the user start sleeping, so the device will calculate the interval between the bedtime alarm and the time the user stop the alarm in order to find the lateness of user's bedtime. In addition, the device will also be able to count the time user spent in learning by detecting the presence of user in their study table and counting the time the user sit there assuming sitting there means studying. Those data will be sent into Thingspeak.com and will be displayed as a graph and the user will be able to evaluate their sleep lateness and study time by seeing it in the app.</p>

<h2>Navigation</h2>
<ol>
  <li><a href="#Overview">Overview</a></li>
  <li><a href="#IoTComp">IoT Components</a></li>
  <li><a href="#Pinout">Pinout Table</a></li>
  <li><a href="#howwork">How Does it Works?</a></li>
</ol>

<h2 class="IoTComp">IoT Components</h2>
<p>The component used in the circuit will be divided into three category, namely the microcontroller, sensor, and actuator: </p>
<h3>Microcontroller</h3>
<ul>
  <li>ESP32</li>
</ul>
<h3>Sensors</h3>
<ul>
  <li>Ultrasonic sensor</li>
  <li>Real Time Clock (RTC module)</li>
  <li>Infrared receiver and infrared remote</li>
</ul>
<h3>Actuators</h3>
<ul>
  <li>Liquid Crystal Display I2C(16x4)</li>
  <li>Red LED</li>
  <li>Buzzer</li>
</ul>

<h2 class="Pinout">Pinout Table</h2>
<h4>ESP32 to Ultrasonic Sensor</h4>
<table>
  <tr>
    <th>ESP32</th>
    <th>Ultrasonic Sensor</th>
  </tr>
  <tr>
    <th>D35</th>
    <th>ECHO</th>
  </tr>
  <tr>
    <th>D32</th>
    <th>TRIG</th>
  </tr>
  <tr>
    <th>GND</th>
    <th>GND</th>
  </tr>
  <tr>
    <th>VCC</th>
    <th>5V</th>
  </tr>
</table>
  
<h4>ESP32 to LED</h4>
<table>
  <tr>
    <th>ESP32</th>
    <th>Red LED</th>
  </tr>
  <tr>
    <td>D33</td>
    <td>Anode</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>Cathode</td>
  </tr>
</table>
  
<h4>ESP32 to IR Receiver</h4>
<table>
  <tr>
    <th>ESP32</th>
    <th>IR Receiver</th>
  </tr>
  <tr>
    <td>D12</td>
    <td>DAT</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
  <tr>
    <td>VCC</td>
    <td>3.3V</td>
  </tr>
</table>

<h4>ESP32 to Buzzer</h4>
<table>
  <tr>
    <th>ESP32</th>
    <th>Buzzer</th>
  </tr>
  <tr>
    <td>D18</td>
    <td>2</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
</table>

<h4>ESP32 to LCD I2C (16x4)</h4>
<table>
  <tr>
    <th>ESP32</th>
    <th>LCD I2C</th>
  </tr>
  <tr>
    <td>D21 (SDA)</td>
    <td>SDA</td>
  </tr>
  <tr>
    <td>D22 (SCL)</td>
    <td>SCL</td>
  </tr>
  <tr>
    <td>VCC</td>
    <td>5V</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
</table>

<h4>ESP32 to RTC Module</h4>
<table>
  <tr>
    <th>ESP32</th>
    <th>RTC Module</th>
  </tr>
  <tr>
    <td>D21 (SDA)</td>
    <td>SDA</td>
  </tr>
  <tr>
    <td>D22 (SCL)</td>
    <td>SCL</td>
  </tr>
  <tr>
    <td>VCC</td>
    <td>5V</td>
  </tr>
  <tr>
    <td>GND</td>
    <td>GND</td>
  </tr>
  <tr>
    <td>-</td>
    <td>SQW</td>
  </tr>
</table>
  
<h2 class="howwork">How does it works?</h2>
