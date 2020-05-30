# Copyright (c) Microsoft. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for
# full license information.

import time
import os
import sys
import asyncio
from six.moves import input
import threading
from azure.iot.device.aio import IoTHubModuleClient
from sense_hat import SenseHat
from datetime import datetime

async def main():
    
    device_client = IoTHubModuleClient.create_from_edge_environment()
    sense = SenseHat()
    sense.clear()

    await device_client.connect()

    async def sense_hat_listener():
        while True:
            time.sleep(5)
            msg = "Temperature: {} at time: {}"
                .format(
                    sense.get_temperature(),
                    datetime.now())
            print(msg)
            await module_client.send_message_to_output(msg, "output1")

        # define behavior for halting the application
        def stdin_listener():
            while True:
                try:
                    selection = input("Press Q to quit\n")
                    if selection == "Q" or selection == "q":
                        print("Quitting...")
                        break
                except:
                    time.sleep(10)

        # Schedule task for C2D Listener
        listeners = asyncio.gather(sense_hat_listener(module_client))

        print ( "Waiting for Sense Hat messages. ")

        # Run the stdin listener in the event loop
        loop = asyncio.get_event_loop()
        user_finished = loop.run_in_executor(None, stdin_listener)

        # Wait for user to indicate they are done listening for messages
        await user_finished

        # Cancel listening
        listeners.cancel()

        # Finally, disconnect
        await module_client.disconnect()

    except Exception as e:
        print ( "Unexpected error %s " % e )
        raise

if __name__ == "__main__":
    asyncio.run(main())