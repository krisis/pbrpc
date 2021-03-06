/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: pbrpc.proto */

#ifndef PROTOBUF_C_pbrpc_2eproto__INCLUDED
#define PROTOBUF_C_pbrpc_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1001001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Pbcodec__PbRpcRequest Pbcodec__PbRpcRequest;
typedef struct _Pbcodec__PbRpcResponse Pbcodec__PbRpcResponse;


/* --- enums --- */


/* --- messages --- */

struct  _Pbcodec__PbRpcRequest
{
  ProtobufCMessage base;
  uint64_t id;
  /*
   * The method string should be in the format "service.method"
   */
  char *method;
  /*
   * Params are protobuf encoded request message for the particualar method
   */
  protobuf_c_boolean has_params;
  ProtobufCBinaryData params;
};
#define PBCODEC__PB_RPC_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&pbcodec__pb_rpc_request__descriptor) \
    , 0, NULL, 0,{0,NULL} }


struct  _Pbcodec__PbRpcResponse
{
  ProtobufCMessage base;
  uint64_t id;
  /*
   * Result is the protobuf encoded response message for the particular method
   */
  ProtobufCBinaryData result;
  char *error;
};
#define PBCODEC__PB_RPC_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&pbcodec__pb_rpc_response__descriptor) \
    , 0, {0,NULL}, NULL }


/* Pbcodec__PbRpcRequest methods */
void   pbcodec__pb_rpc_request__init
                     (Pbcodec__PbRpcRequest         *message);
size_t pbcodec__pb_rpc_request__get_packed_size
                     (const Pbcodec__PbRpcRequest   *message);
size_t pbcodec__pb_rpc_request__pack
                     (const Pbcodec__PbRpcRequest   *message,
                      uint8_t             *out);
size_t pbcodec__pb_rpc_request__pack_to_buffer
                     (const Pbcodec__PbRpcRequest   *message,
                      ProtobufCBuffer     *buffer);
Pbcodec__PbRpcRequest *
       pbcodec__pb_rpc_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   pbcodec__pb_rpc_request__free_unpacked
                     (Pbcodec__PbRpcRequest *message,
                      ProtobufCAllocator *allocator);
/* Pbcodec__PbRpcResponse methods */
void   pbcodec__pb_rpc_response__init
                     (Pbcodec__PbRpcResponse         *message);
size_t pbcodec__pb_rpc_response__get_packed_size
                     (const Pbcodec__PbRpcResponse   *message);
size_t pbcodec__pb_rpc_response__pack
                     (const Pbcodec__PbRpcResponse   *message,
                      uint8_t             *out);
size_t pbcodec__pb_rpc_response__pack_to_buffer
                     (const Pbcodec__PbRpcResponse   *message,
                      ProtobufCBuffer     *buffer);
Pbcodec__PbRpcResponse *
       pbcodec__pb_rpc_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   pbcodec__pb_rpc_response__free_unpacked
                     (Pbcodec__PbRpcResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Pbcodec__PbRpcRequest_Closure)
                 (const Pbcodec__PbRpcRequest *message,
                  void *closure_data);
typedef void (*Pbcodec__PbRpcResponse_Closure)
                 (const Pbcodec__PbRpcResponse *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor pbcodec__pb_rpc_request__descriptor;
extern const ProtobufCMessageDescriptor pbcodec__pb_rpc_response__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_pbrpc_2eproto__INCLUDED */
