OnboardCommLayer
======================

Communication layer to run on the nodes.  Hides the request-based i2c protocol by queuing messages for sending.
This allows the experimental sampling to be decoupled from the request loop.
Imported checksumming on the data.
