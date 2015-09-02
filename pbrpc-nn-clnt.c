#include "pbrpc.pb-c.h"
#include "pbrpc-clnt.h"

#include <string.h>
#include <stdlib.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <pthread.h>

static uint64_t
next_id (void)
{
        static uint64_t id_gen = 0;
        static pthread_mutex_t id_lock = PTHREAD_MUTEX_INITIALIZER;

        uint64_t id = 0;

        pthread_mutex_lock (&id_lock);
        {
                id = ++id_gen;
        }
        pthread_mutex_unlock (&id_lock);

        return id;
}

pbrpc_clnt *
pbrpc_clnt_new (const char *host, int16_t port)
{
        pbrpc_clnt *clnt = NULL;
        char caddr[256] = {0};

        clnt = calloc (1, sizeof (*clnt));
        if (!clnt)
                return NULL;

        clnt->sock = nn_socket (AF_SP, NN_REQ);
        assert (clnt->sock != -1);

        snprintf (caddr, sizeof (caddr), "tcp://%s:%d", host, port);
        assert (nn_connect (clnt->sock, caddr) != -1);
        return clnt;
}

int
pbrpc_clnt_destroy (pbrpc_clnt *clnt)
{
        nn_close (clnt->sock);
        free (clnt);
}

int
pbrpc_clnt_call (pbrpc_clnt *clnt, const char *method, ProtobufCBinaryData *msg,
                 ProtobufCBinaryData *rep, int *status)
{
        Pbcodec__PbRpcRequest reqhdr = PBCODEC__PB_RPC_REQUEST__INIT;
        Pbcodec__PbRpcResponse *rsp = NULL;
        int       ret = -1;
        size_t    rlen = 0;
        int       nbytes;
        char     *rbuf = NULL;
        char     *mth  = NULL;

        mth = strdup (method);
        if (!mth)
                return -1;

        reqhdr.id = next_id();
        reqhdr.has_params = 1;
        reqhdr.params.data = msg->data ;
        reqhdr.params.len = msg->len;
        reqhdr.method = mth;

        rlen = rpc_write_request (clnt, &reqhdr, &rbuf);

        nbytes = nn_send (clnt->sock, rbuf, rlen, 0);
        assert (nbytes == rlen);
        free (rbuf);

        nbytes = nn_recv (clnt->sock, &rbuf, NN_MSG, 0);
        assert (nbytes != -1);
        rsp = rpc_read_rsp (rbuf, nbytes);
        if (!rsp) {
                fprintf (stderr, "Failed to parse response "
                         "from server\n");
                *status = -1;
                return -1;
        }

        rep->len = rsp->result.len;
        rep->data = calloc (1, rep->len);
        memcpy (rep->data, rsp->result.data, rep->len);
        pbcodec__pb_rpc_response__free_unpacked (rsp, NULL);
        nn_freemsg (rbuf);
        free (mth);
        *status = 0;
        return 0;
}
