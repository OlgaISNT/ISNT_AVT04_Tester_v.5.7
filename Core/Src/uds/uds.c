#include "uds.h"
//#include <bitfield/bitfield.h>
//#include <canutil/read.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include <sys/param.h>
#include <inttypes.h>
#include "FreeRTOS.h"
#include "task.h"
//#include "task.h"
//#include <../.h>
#include "IVI_Can_Matrix.h"
#include "IVI_Diagnostic.h"
#include "main.h"
#include "semphr.h"
#include "isotp/isotp_types.h"
//#include "main.h"

#define ARBITRATION_ID_OFFSET 0x8
#define MODE_RESPONSE_OFFSET 0x40
#define NEGATIVE_RESPONSE_MODE 0x7f
#define MAX_DIAGNOSTIC_PAYLOAD_SIZE 6
#define MODE_BYTE_INDEX 0
#define PID_BYTE_INDEX 1
#define NEGATIVE_RESPONSE_MODE_INDEX 1
#define NEGATIVE_RESPONSE_NRC_INDEX 2

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
extern DiagnosticRequest request;
extern DiagnosticResponse response;
 can_message_t can_message;
extern IsoTpMessage message ;
//extern IsoTpMessage message;
//q cnf_ecu  CNF_ECU;
// cnf_car CNF_CAR;
extern CAN_TxHeaderTypeDef TxHeader;
extern  CAN_RxHeaderTypeDef RxHeader;
extern can_message_t recvMsg;
extern can_message_t mrecvMsg;
//extern CAN_HandleTypeDef hcan1;
//SemaphoreHandle_t   gate;
uint8_t combined_payload[64];

bool CAN_UDS_Request_Parse(  IsoTpMessage *mess , can_message_t  *handle ,
					     DiagnosticRequest* request,
						DiagnosticResponse* response ,
						uint32_t arbitration_id) {



		uint16_t payload_length;

		 mess->completed= false;

		switch (handle->data[0] & 0xf0) {

	case PCI_SINGLE:
		request->pid_length = (handle->data[0] & 0x0f);

		if (request->pid_length > 0) {
			memcpy(&mess->payload[0], &handle->data[1], request->pid_length);
		}

		mess->size = request->pid_length;
		//  message.completed = true;

	//	request->arbitration_id = handle->data[1];  //;0x2e
		response->arbitration_id = handle->data[1];

	if(response->arbitration_id == (request->arbitration_id + 0x40))     // answer
		{

		///////////subindex

		response->subindex = handle->data[2];

             if(response->subindex == request->subindex)
              {
                 response-> completed = true;
                 response->success = true;
               }

		     ///////////cmd_id
		      request->cmd_id = handle->data[3];
		      response->cmd_id = request->cmd_id;

	      	}

	else {

	       response->negative_response_code = handle->data[3];  // 0x3  7F 22 31
		   response-> completed = false;
		   response->success = false;
		         if(response->negative_response_code != 0x21) //// busy
		      ; //         Can_state.state = 0x0;
                     	}
	       if(response-> arbitration_id == OBD2_CMD_WRITE)      // if Request arbitration_id = 0x22 we send a Response
	         {
              CAN_UDS_Diagnostic_Send(request, response);
	           }
		request->pct_num = 0;
		//		isotp_handle_single_frame(handle, &message);

		////old

		break;

		case PCI_FIRST_FRAME:

			request->pct_num = 1;

		payload_length = sizeof(handle->data); //(get_nibble(data, size, 1) << 8) + get_byte(data, size, 1);
		request->payload_length = (handle->data[1]);

		if (request->pid_length > 0x8) {

			request->ltimer = xTaskGetTickCount();
			request->succes = false;
		}
		if (payload_length > OUR_MAX_ISO_TP_MESSAGE_SIZE) {
			// shims->log("Multi-frame response too large for receive buffer.");
			break;
		}

		//Need to allocate memory for the combination of multi-frame
		//messages. That way we don't have to allocate 4k of memory
		//for each multi-frame response.



	//	memcpy(combined_payload, &handle->data[2], CAN_MESSAGE_BYTE_SIZE - 2);
		//      message.payload = combined_payload;
		memcpy(&combined_payload[0], &handle->data[2], CAN_MESSAGE_BYTE_SIZE - 2);
		request->receive_buffer = combined_payload;
		request->received_buffer_size = CAN_MESSAGE_BYTE_SIZE - 2;
		request->incoming_message_size = request->payload_length;

		mess->multi_frame = true;
		request->completed = false;
		request->succes = false;

		//arbitration_id
		request->arbitration_id = handle->data[2];  //;0x2e
		response->arbitration_id = request->arbitration_id;

		///////////subindex
		request->subindex = handle->data[3];   //0xa3
		response->subindex = request->subindex;

		///////////cmd_id
		request->cmd_id = handle->data[4];    //04
		response->cmd_id = request->cmd_id;
		//!!!!!!!!
//		free(request->receive_buffer);
		//	request->pct_num = 0;
		IVI_Send_Diagnostic_Single_Response(8, FLOW_CONTROL_ON ,1);



			break;

		case PCI_FLOW_CONTROL_FRAME:
			response->flow = true;
			break;


	    case PCI_CONSECUTIVE_FRAME: {
	            uint8_t start_index = request->received_buffer_size;
	            uint8_t remaining_bytes = request->incoming_message_size - start_index;
	            mess->multi_frame = true;
	            request->pct_num  +=1;


	           if(remaining_bytes > 7) {
	                memcpy(&request->receive_buffer[start_index], &handle->data[1], CAN_MESSAGE_BYTE_SIZE - 1);
	                request->received_buffer_size = start_index + 7;
	                IVI_Send_Diagnostic_Single_Response(
	                				8, FLOW_CONTROL_ON ,1);

	             //   IVI_Send_Diagnostic_Response(&can_pal1_instance, IVI_DTOOL_to_Radio_ID, 8, FLOW_CONTROL_ON);
	            } else {
	                memcpy(&request->receive_buffer[start_index], &handle->data[1], remaining_bytes);
	                request->received_buffer_size = start_index + remaining_bytes;

	                if(request->received_buffer_size != request->incoming_message_size){
	              //      free(request->receive_buffer);
	                    request->succes = false;
	                    IVI_Send_Diagnostic_Single_Response(8, ANSWER_NO_ON ,1);


	                } else {
                        mess->size = request->incoming_message_size;
	                    mess->completed = true;

	                    memcpy(&mess->payload[0],  &request->receive_buffer[0], request->incoming_message_size);
	                    request->succes = true;
	                    request->completed = true;
	                    request->pct_num = 0;
	                    response->completed = true;
	                    response->success = true;


	                }
	            }

	            break;
	    }
		default: // thread
		//	request.cmd_id = 0;

			break;

		}

		return response->success;
//return 1;
	}





status_t CAN_UDS_Diagnostic_Send (
						DiagnosticRequest* req,
						DiagnosticResponse* resp

						)
 {
	status_t status;


		 uint8_t start_index = 0;
		 uint8_t remaining_bytes = message.size - start_index;

		// resp->mode = PCI_FIRST_FRAME;

		   can_message_t handle = {
				   .cs =  TxHeader.ExtId,//0U,
				  .id =  TxHeader.StdId,//778,
				  .length = TxHeader.DLC,
			            };

		memset(handle.data, 0 , sizeof(handle.data));

		switch (resp->mode) {

				case PCI_SINGLE:
		       memcpy(&handle.data[0], &message.payload[start_index], 8);
					 	      request.succes = true;
			//   status = IVI_Send_Frame  (  &handle);

					break;

				case PCI_FIRST_FRAME:

					resp->pct_num = 1;
					memcpy(&handle.data[0], &message.payload[start_index], CAN_MESSAGE_BYTE_SIZE );
					status = IVI_Send_Frame  (  &handle);
					resp->ltimer = xTaskGetTickCount();
					resp->success = false;
					start_index += CAN_MESSAGE_BYTE_SIZE ;
					resp->flow = true;
					vTaskDelay(request.STmin);
					//vTaskDelay(100);
				if(	CAN_UDS_Diagnostic_Response_Wait(resp, PCI_FLOW_CONTROL_FRAME , N_As *1000))
		  		{
					resp->mode = PCI_CONSECUTIVE_FRAME;
		  		}

					do{


						 remaining_bytes = message.size - start_index;

						handle.data[0]= (0x20 | resp->pct_num );

						memcpy(&handle.data[1], &message.payload[start_index], remaining_bytes);

					    status = IVI_Send_Frame  ( &handle);
						start_index += CAN_MESSAGE_BYTE_SIZE - 1;
						//resp->ltimer = xTaskGetTickCount();
						resp->success = false;
						resp->pct_num +=1;
						if(resp->pct_num == 0x0f)  resp->pct_num = 0;

						vTaskDelay(request.STmin);

					}

					while (remaining_bytes > 7);
			    	resp->success = true;
					resp->completed = true;
					resp->pct_num = 0;
					 break;



				default: // thread
				//resp->cmd_id = 0;

				break;

							/////request.pid_length
		}

return status;
	}




status_t  IVI_Send_Diagnostic_Single_Response( /*IsoTpMessage *mess,*/
     								 uint32_t dlc, uint32_t event , uint32_t send )
	{
	status_t status;

     can_message_t message = {
    		 .cs =  TxHeader.ExtId,//0U,
    		 .id =  TxHeader.StdId,//778,
    		 .length = TxHeader.DLC,
         		          };

    	   switch (event){

 	       case FLOW_CONTROL_ON:


 	      	message.data[0] = (0x30 /*| (request.FS & 0xf)*/);
 	   		message.data[1] = request.BS;
 	   		message.data[2] = request.STmin;
 	   		memset(&message.data[3], 0xaa, (message.length - 3));


          break;

 	      case ANSWER_OK_ON:

 	      message.data[0] =  ((PCI_SINGLE ) | 0x8) ;
 	      message.data[1] = request.arbitration_id + 0x40; //request.arbitration_id + 0x40;;
 	      message.data[2] = request.subindex; //request.arbitration_id + 0x40;

 	      message.data[3] = request.cmd_id;
 	      message.data[4] = OBD2_ANSWER_OK;
 	    	//memset(&message.data[5], 0xaa, (message.length - 5));
 	   //    CAN_UDS_diagnostic_Cmd_message(&message, &request, OBD2_CMD_WRITE);
 	      request.succes = true;
 	      break;

 	      	case ANSWER_NO_ON:

 	      	  message.data[0] = ((PCI_SINGLE )| 0x8) ;
 	      	  message.data[1] = request.arbitration_id ;;
 	      	  message.data[2] = 0x7F; //request.arbitration_id + 0x40;
 	      	  message.data[3] = response.negative_response_code;
   	      	 request.succes = true;
           break;

 	      default:

 	      	break;

    	 }
   //  }

     if(send)
          status = IVI_Send_Frame  (  &message);

return status;
	}


