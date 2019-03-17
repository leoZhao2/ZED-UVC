///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2018, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


#include <sl_zed/Camera.hpp>

using namespace sl;

int main(int argc, char **argv) {

	// Create a ZED camera object
	Camera zed;

	// Set configuration parameters
	InitParameters init_params;
	init_params.camera_resolution = RESOLUTION_HD720; // Use HD720 video mode (default fps: 60)
	init_params.coordinate_system = COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP; // Use a right-handed Y-up coordinate system
	init_params.coordinate_units = UNIT_METER; // Set units in meters


	// Open the camera
	ERROR_CODE err = zed.open(init_params);
	if (err != SUCCESS)
		exit(-1);

	// Enable positional tracking with default parameters
	TrackingParameters tracking_parameters;
	err = zed.enableTracking(tracking_parameters);
	if (err != SUCCESS)
		exit(-1);


	// Track the camera position during 1000 frames
	int i = 0;
	Pose zed_pose;

	// Check if the camera is a ZED M and therefore if an IMU is available
	bool zed_mini = (zed.getCameraInformation().camera_model == MODEL_ZED_M);
	IMUData imu_data;

	while (i < 1000) {
		if (zed.grab() == SUCCESS) {
			zed.getPosition(zed_pose, REFERENCE_FRAME_WORLD); // Get the pose of the left eye of the camera with reference to the world frame

			// Display the translation and timestamp
			printf("\nTranslation: Tx: %.3f, Ty: %.3f, Tz: %.3f, Timestamp: %llu\n", zed_pose.getTranslation().tx,
				zed_pose.getTranslation().ty, zed_pose.getTranslation().tz, zed_pose.timestamp);

			// Display the orientation quaternion
			printf("Orientation: Ox: %.3f, Oy: %.3f, Oz: %.3f, Ow: %.3f\n", zed_pose.getOrientation().ox,
				zed_pose.getOrientation().oy, zed_pose.getOrientation().oz, zed_pose.getOrientation().ow);


			if (zed_mini) { // Display IMU data

				 // Get IMU data
				zed.getIMUData(imu_data, TIME_REFERENCE_IMAGE);

				// Filtered orientation quaternion
				printf("IMU Orientation: Ox: %.3f, Oy: %.3f, Oz: %.3f, Ow: %.3f\n", imu_data.getOrientation().ox,
					imu_data.getOrientation().oy, imu_data.getOrientation().oz, zed_pose.getOrientation().ow);
				// Raw acceleration
				printf("IMU Acceleration: x: %.3f, y: %.3f, z: %.3f\n", imu_data.linear_acceleration.x,
					imu_data.linear_acceleration.y, imu_data.linear_acceleration.z);
			}

			i++;
		}
	}

	// Disable positional tracking and close the camera
	zed.disableTracking();
	zed.close();
	return 0;
}
