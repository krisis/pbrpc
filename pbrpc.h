#ifndef _RPC_H
#define _RPC_H

#include "pbrpc.pb-c.h"

/**
 * Rpcsvc method type
 */
typedef int (*pbrpc_svc_fn) (ProtobufCBinaryData *req,
                          ProtobufCBinaryData *rsp);
/**
 * Rpcsvc method entry
 */
struct pbrpc_svc_fn_obj {
        pbrpc_svc_fn fn;
        char *name;
};

typedef struct pbrpc_svc_fn_obj pbrpc_svc_fn_obj;

struct pbrpc_svc {
        int sock;
        void *ctx;
        pbrpc_svc_fn_obj *methods;
};

typedef struct pbrpc_svc pbrpc_svc;


/**
 * Creates a new pbrpc based service listening at hostname on port
 *
 * @param hostname - a string representing the hostname or IP address of the
 *                 node
 *
 * @param port - the port on which the service needs to listening
 *
 * @return  - a pointer to a newly allocated pbrpc_svc object, or NULL if an error
 *            occurred
 *
 * */

pbrpc_svc*
pbrpc_svc_new (const char *hostname, int16_t port);


/**
 * Registers RPC methods with a pbrpc_svc object
 *
 * @param svc - the service object
 * @param methods - a list of functions that are exported via @pbrpc_svc
 * @return - 0 on success, -1 on failure
 *
 * */
int
pbrpc_svc_register_methods (pbrpc_svc *svc, pbrpc_svc_fn_obj *methods);


/**
 * Starts listening for new requests
 *
 * @param svc - the service object
 * @return - 0 on success, -1 on failure
 *
 * */
int
pbrpc_svc_serve (pbrpc_svc *svc);


/**
 * Closes the listen endpoint and frees @svc
 * @param svc - the service object
 * @return - 0 on success, -1 on failure
 *
 * */
int
pbrpc_svc_destroy (pbrpc_svc* svc);


typedef int (*rpc_handler_func) (ProtobufCBinaryData *req,
                                 ProtobufCBinaryData *reply);

Pbcodec__PbRpcRequest *
rpc_read_req (pbrpc_svc *svc, const char* msg, size_t msg_len);

int
rpc_write_reply (pbrpc_svc *svc, Pbcodec__PbRpcResponse *rsphdr, char **buf);

int
rpc_invoke_call (pbrpc_svc *svc, Pbcodec__PbRpcRequest *reqhdr,
                 Pbcodec__PbRpcResponse *rsphdr);

#endif
