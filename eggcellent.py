from inference import get_model
import supervision as sv
import cv2
import time
import logging
import os
import serial

def connect_to_arduino():
    attempt = 0
    while attempt < 5:  # Retry up to 5 times
        try:
            arduino = serial.Serial('COM3', 9600)  # Use the correct COM port
            time.sleep(2)  # Wait for the connection to establish
            print("Arduino connected!")
            return arduino
        except serial.SerialException as e:
            print(f"Error: {e}")
            attempt += 1
            time.sleep(1)  # Wait before retrying
    print("Failed to connect to Arduino after 5 attempts.")
    return None

arduino = connect_to_arduino()

if arduino:
    # Send a test message to the Arduino
    arduino.write(b'Hello from Python!')
    print("Hello Arduino!")
else:
    print("Unable to establish serial connection.")

# Fetch the Roboflow API key
api_key = "BMSa7BW3SA0MoXjwOqDO"

log_dir = "C:\\Users\\HP\\OneDrive\\Documents\\Eggcellent Logs"  # Windows path
if not os.path.exists(log_dir):
    os.makedirs(log_dir)  # Create the directory if it doesn't exist

log_file = os.path.join(log_dir, "detection_log.txt")

# Delete the log file if it exists (when the app starts)
if os.path.exists(log_file):
    os.remove(log_file)  # Remove the existing log file

# Set up logging with the custom file path
logging.basicConfig(
    filename=log_file,
    level=logging.INFO,
    format='%(asctime)s - %(message)s'
)

print(f"Logging to: {log_file}")

# Load a pre-trained yolov8n model
model = get_model(model_id="itlog-ni-adongis/4", api_key=api_key)

# Open the camera (0 for default camera, or 1, 2, etc., for external cameras)
camera = cv2.VideoCapture(1)

# Check if the camera is opened successfully
if not camera.isOpened():
    print("Error: Could not open the camera.")
    exit()

# Create supervision annotators
bounding_box_annotator = sv.BoxAnnotator()
label_annotator = sv.LabelAnnotator()

print("Press 'q' to quit.")

last_detection_time = time.time()

# Define the function to send log data to Arduino
def send_log_to_arduino(arduino, log_message):
    if arduino and arduino.is_open:
        try:
            arduino.write(log_message.encode())  # Send each log line to the Arduino
        except serial.SerialException as e:
            print(f"Error sending data: {e}")
    else:
        print("Serial port is not open!")

def printDetection(prediction):
    # Log the detection message
    log_message = f"{prediction.class_name} Egg detected: {prediction}"
    
    # Only send the log to the Arduino when an object is detected
    send_log_to_arduino(arduino, log_message)
    print(log_message)
    logging.info(log_message)

try:
    while True:
        # Capture frame-by-frame
        ret, frame = camera.read()
        if not ret:
            print("Failed to capture frame.")
            break

        # Run inference on the frame
        inference_result = model.infer(frame)[0]

        # Access predictions attribute
        predictions = inference_result.predictions  # Assuming predictions is an attribute

        # Check if the time interval of 5 seconds has passed
        current_time = time.time()
        if current_time - last_detection_time >= 5:
            # Update the last detection time
            last_detection_time = current_time

            # Print results for "egg" and "crack" detections
            for prediction in predictions:
                class_name = getattr(prediction, "class_name", None)  # Access class_name attribute                
                if class_name == "Good":
                    printDetection(prediction) 
                if class_name == "Bad":                    
                    printDetection(prediction) 
                if class_name == "Error":          
                    printDetection(prediction)

        # Load the results into the supervision Detections API
        detections = sv.Detections.from_inference(inference_result)

        # Annotate the frame with inference results
        annotated_frame = bounding_box_annotator.annotate(
            scene=frame, detections=detections
        )
        annotated_frame = label_annotator.annotate(
            scene=annotated_frame, detections=detections
        )

        # Display the annotated frame
        cv2.imshow('Real-Time Inference', annotated_frame)

        # Press 'q' to quit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
except KeyboardInterrupt:
    print("Interrupted by user.")

# Release the camera and close all OpenCV windows
arduino.close()
camera.release()
cv2.destroyAllWindows()
