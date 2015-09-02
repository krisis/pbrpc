#include "pbrpc.h"
#include "pbrpc.pb-c.h"

#include <stdio.h>
#include <stdlib.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>


pbrpc_svc *
pbrpc_svc_new (const char *hostname, int16_t port)
{
        pbrpc_svc *svc = NULL;
        char bindaddr[256] = {0};

        svc = calloc (1, sizeof (*svc));
        if (!svc)
                return NULL;

        svc->sock = nn_socket (AF_SP, NN_REP);
        assert (svc->sock >= 0);
        snprintf (bindaddr, sizeof(bindaddr), "tcp://%s:%d", hostname, port);
        nn_bind (svc->sock, bindaddr);
        return svc;
}


int
pbrpc_svc_destroy (pbrpc_svc *svc)
{
        nn_close (svc->sock);
        free (svc);
}


int
pbrpc_svc_register_methods (pbrpc_svc *svc, pbrpc_svc_fn_obj *methods)
{
        svc->methods = methods;
        return 0;
}


static int
svc_handle_call (pbrpc_svc *svc, const char *buf, uint64_t len)
{

        int                     ret     = -1;
        char                    *outbuf = NULL;
        Pbcodec__PbRpcRequest   *reqhdr = NULL;
        Pbcodec__PbRpcResponse  rsphdr  = PBCODEC__PB_RPC_RESPONSE__INIT;

        reqhdr = rpc_read_req (svc, buf, len);
        ret = rpc_invoke_call (svc, reqhdr, &rsphdr);
        if (ret) {
                fprintf(stderr, "ret = %d: rpc_invoke_call failed\n", ret);
        }

        ret = rpc_write_reply (svc, &rsphdr, &outbuf);
        if (ret <= 0) {
                fprintf(stderr, "ret = %d: rpc_write_reply failed\n", ret);
        }

        pbcodec__pb_rpc_request__free_unpacked (reqhdr, NULL);
        assert (nn_send (svc->sock, outbuf, ret, 0) == ret);
}


int
pbrpc_svc_serve (pbrpc_svc *svc)
{
        char *buf;
        int64_t nbytes;

        for (;;) {
                nbytes = nn_recv (svc->sock, &buf, NN_MSG, 0);
                svc_handle_call (svc, buf, nbytes);
                nn_freemsg(buf);
        }
}
