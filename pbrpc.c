#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <errno.h>
#include <arpa/inet.h>


#include "pbrpc.h"
#include "pbrpc-clnt.h"
#include "pbrpc.pb-c.h"

/*
 * RPC Format
 * -----------------------------------------------------
 * | Length: uint64                                    |
 * -----------------------------------------------------
 * |                                                   |
 * | Header:  Id uint64, Method int32, Params []bytes |
 * |                                                   |
 * -----------------------------------------------------
 * |                                                   |
 * | Body: Method dependent representation             |
 * |                                                   |
 * -----------------------------------------------------
 *
 **/

/*
 * pseudo code:
 *
 * read buffer from network
 * reqhdr = rpc_read_req (svc, msg, len);
 * rsphdr = RSP_HEADER__INIT;
 * ret = rpc_invoke_call (svc, reqhdr, &rsphdr)
 * if (ret)
 *   goto out;
 * char *buf = NULL;
 * ret = rpc_write_reply (svc, &rsphdr, &buf);
 * if (ret != -1)
 *   goto out;
 *
 * send buf over n/w
 *
 * free(buf);
 * free(rsp->result->data);
 *
 *
 * ...
 * clean up protobuf buffers on finish
 *
 * */

/* returns the no. of bytes written to @buf
 * @buf is allocated by this function.
 * */
int
rpc_write_request (pbrpc_clnt *clnt, Pbcodec__PbRpcRequest *reqhdr, char **buf)
{
        uint64_t be_len = 0;
        if (!buf)
                return -1;

        size_t reqlen = pbcodec__pb_rpc_request__get_packed_size (reqhdr);
        *buf = calloc (1, reqlen+8);
        if (!*buf)
                return -1;

        pbcodec__pb_rpc_request__pack(reqhdr, *buf+8);
        be_len = htobe64 (reqlen);
        memcpy(*buf, &be_len, sizeof(uint64_t));
        return reqlen + sizeof(uint64_t);
}


/* returns the no. of bytes written to @buf
 * @buf is allocated by this function.
 * */
int
rpc_write_reply (pbrpc_svc *svc, Pbcodec__PbRpcResponse *rsphdr, char **buf)
{
        uint64_t be_len = 0;
        if (!buf)
                return -1;

        size_t rsplen = pbcodec__pb_rpc_response__get_packed_size (rsphdr);
        *buf = calloc (1, rsplen+sizeof(uint64_t));
        if (!*buf)
                return -1;

        be_len = htobe64 (rsplen);
        pbcodec__pb_rpc_response__pack (rsphdr, *buf + sizeof(uint64_t));
        memcpy (*buf, &be_len, sizeof(be_len));
        return rsplen+sizeof(uint64_t);
}


int
rpc_invoke_call (pbrpc_svc *svc, Pbcodec__PbRpcRequest *reqhdr,
                 Pbcodec__PbRpcResponse *rsphdr)
{
        int ret;

        if (!reqhdr->has_params){
                fprintf(stderr, "no params passed\n");
                return -1;
        }
        rsphdr->id = reqhdr->id;

        pbrpc_svc_fn_obj *method;
        for (method = svc->methods; method; method++)
                if (!strcmp (method->name, reqhdr->method))
                        break;

        if (!method)
                return -1;

        ret = method->fn (&reqhdr->params, &rsphdr->result);

        return ret;
}


Pbcodec__PbRpcResponse *
rpc_read_rsp (const char *msg, size_t msg_len)
{
        char *hdr;
        uint64_t proto_len = 0;

        memcpy (&proto_len, msg, sizeof(uint64_t));
        proto_len = be64toh (proto_len);

        return pbcodec__pb_rpc_response__unpack(NULL, proto_len, msg+sizeof(proto_len));
}


Pbcodec__PbRpcRequest *
rpc_read_req (pbrpc_svc *svc, const char* msg, size_t msg_len)
{
        char *hdr;
        uint64_t proto_len = 0;

        memcpy (&proto_len, msg, sizeof(uint64_t));
        proto_len = be64toh (proto_len);
        return pbcodec__pb_rpc_request__unpack (NULL, proto_len, msg+sizeof(proto_len));
}
