SAT_AppStorage
==========

SAT_AppStorage is a Nanosatisfi library that provides access to long-term experimental storage device.
An arduino program running on the satellite sends a string to to its supervisor
to be written to disk.
When experiment reaches its predefined limit of 10KB, the experiment will exit
and flush remaining data to satellite. This will start the downloading process.
Otherwise when a user sends a message to the Supervisor to invoke exit.


