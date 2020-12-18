# Home-Automation-MQTT

Alat yang digunakan :
  1. NodeMCU esp8266
  2. Wemos D1 mini
  3. Modul Relay 4 channel
  4. Clipsal Switch 4 tombol
  
  
Deskripsi Alat
  Home Automation System adalah aplikasi gabungan antara teknologi dan pelayanan yang dikhusukan pada lingkungan rumah dengan fungsi tertentu yang bertujuan meningkatkan keamanan, efisiensi, dan kenyamanan penghuninya. Sistem rumah pintar (smart home) biasanya terdiri dari perangkat monitoring, perangkat kontrol dan otomatis ada beberapa perangkat yang dapat di akses menggunakan Microcontroller.
Home Automation System merupakan aplikasi yang dapat di rancang dengan bantuan Microcontroller yang dapat memberikan keamanan, kemudahan, dan dapat menghemat energi listrik yang berlangsung otomatis sesuai dengan kendali pengguna dan terprogram melalui Microcontroller pada tempat tinggal kita.
Alat Home Automation ini Menggunakan 2 perangkat yaitu rangkaian saklar sebagai input data dan rangkaian output lampu sebagai output dari alat Home Automation. Pada alat Home Automation ini telah terintegrasi oleh RMQTT Broker, dimana RMQ ini berfungsi untuk mengirim data input (Message) saklar menuju Broker, lalu perangkat output (Client) akan mengambil data input tersebut dari Broker kemudian di olah untuk memberikan output yang di harapkan.
