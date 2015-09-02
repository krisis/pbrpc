#include "calc.pb-c.h"
#include "../pbrpc.h"
#include "../pbrpc-clnt.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#define DBUG(x) fprintf (stdout, "%s was called\n", # x)

int main(int argc, char **argv)
{
        int ret;
        pbrpc_clnt *clnt = NULL;

        clnt = pbrpc_clnt_new ("localhost", 9876);
        if (!clnt) {
                fprintf (stderr, "Failed to create an pbrpc_clnt object\n");
                return 1;
        }

        Calc__CalcReq calc = CALC__CALC_REQ__INIT;
        calc.op = 1; calc.a = 2; calc.b = 3;
        size_t clen = calc__calc_req__get_packed_size(&calc);
        char *cbuf = calloc (1, clen);
        calc__calc_req__pack(&calc, cbuf);

        ProtobufCBinaryData msg;
        msg.len = clen;
        msg.data = cbuf;


        int i;
        for (i = 0; i < 10; i++) {
                ProtobufCBinaryData rep;
                ret = pbrpc_clnt_call (clnt, "calculate", &msg, &rep, &ret);
                if (ret) {
                        fprintf (stderr, "RPC call failed\n");
                }

                Calc__CalcRsp *crsp = calc__calc_rsp__unpack (NULL, rep.len,
                                                              rep.data);

                printf("rsp->sum = %d\n", crsp->ret);
                calc__calc_rsp__free_unpacked (crsp, NULL);
                free (rep.data);
                rep.data = NULL;
        }

        return 0;
}



