## SuperSatsSwitch - A IoT switch to SuperSats.tv

As you get satoshis from the audience, you may turn on anything you want, including lighting, articulated dolls, and other real-world things, using a switch similar to the BitcoinSwitch.

TO-DO:
> Publish the code. B'p

> Finish the installation instructions

### Includes portal for easy setup
-
-

### Install instructions

- 
- 
- 
- 

### Heavily inspired by:

- <a href="https://github.com/lnbits/bitcoinswitch">Bitcoin Switch</a>
- <a href="https://github.com/BitMaker-hub/NerdMiner_v2">NerdMinerV2</a>

Invoice example: lnbc1u1pj3xa24pp5lrezz9q3n89r6l0yh74uucfuwvq7n30u63jf07l40mdwar82p2vqdp8wd6hqetjwdshgueq9pmxjcfqf389qs2e9e3k72gcqzzsxqyz5vqsp59uyv2er0edt4rele5lp0evz5hq96zpg83q686884wp8wq7a48f6s9qyyssqxfem0dr7e6zezhcu6gga0czmru7ln976t6gutlfmpf5w22j6ntf9s6u007tr9jkzgyvvsgrg3rcz90g5h3jgfejy8x5wwmwqqhudekgpqc6ye7

{
    "path": "/socket.io/",
    "agent": false,
    "withCredentials": false,
    "upgrade": true,
    "timestampParam": "t",
    "rememberUpgrade": false,
    "rejectUnauthorized": true,
    "perMessageDeflate": {
        "threshold": 1024
    },
    "transportOptions": {},
    "closeOnBeforeunload": true,
    "hostname": "api.supersats.tv",
    "secure": true,
    "port": "443"
}

[
    "new-message",
    {
        "id": 1353,
        "chat_id": 35,
        "user_id": null,
        "lntx_id": "lntx_w1XUMJonnYslSIWSjBmsrBYE",
        "transaction_id": "wtx_mqrGtliBiU8VjzZBic2maI",
        "num_satoshis": 100,
        "status": 1,
        "message": "Mais%20SuperChats!",
        "created_at": "2023-09-26T22:47:27.158-03:00",
        "updated_at": "2023-09-26T22:47:27.158-03:00",
        "viewed_at": null
    }
]

wss://api.supersats.tv/socket.io/?EIO=4&transport=websocket&sid=ACmog6039t0iIHOmAABF

https://api.lnpay.co/v1/wallet/wal_7ihvT2Bnkq6UYi/transactions

[{"id":"wtx_mqrGtliBiU8VjzZBic2maI","created_at":1695779247,"num_satoshis":100,"user_label":"supersats (via LNPAY.co)","wal":{"id":"wal_7ihvT2Bnkq6UYi","created_at":1689884348,"updated_at":1695779247,"user_label":"SuperSats","balance":302,"balance_msat":null,"ln_node_id":"lnod_do63ikmrhktqnpj9v","default_lnurlpay_id":"lnurlp_VSifV7LMUCOOxgKn0w","statusType":{"type":"wallet","name":"active","display_name":"Active"},"walletType":{"name":"generic_wallet","display_name":"Generic Wallet"}},"wtxType":{"layer":"ln","name":"ln_deposit","display_name":"LN Deposit"},

"lnTx":
{
    "id":"lntx_w1XUMJonnYslSIWSjBmsrBYE","created_at":1695779227,"ln_node_id":"lnod_do63ikmrhktqnpj9v",dest_pubkey":"0254e7f2b9d3b3e23a24e342f2eb631f7f3727c9ee1a6bea07aca792d1e1c24741","payment_request":"lnbc1u1pj38zvmpp5nmx3rl0dzdgjh239n73mpuc3eqsddg72g2af6jtz4zha0zzq9f7sdp8wd6hqetjwdshgueq9pmxjcfqf389qs2e9e3k72gcqzzsxqyz5vqsp5l28s2xmfgznn35ny69ynl2j6h44esy4vsgvnk3h674nnrsupdkrs9qyyssqyhln9he8h3wjk3dx3duzjz4cg65extm70z5ej39tdqvy27442lk4aw6k8s8mtmtljkxgqq05mzrcljwu8x9xngwzrxnz3zskc966gtgqshlfap","r_hash_decoded":"9ecd11fded13512baa259fa3b0f311c820d6a3ca42ba9d4962a8afd788402a7d",
    "memo":"supersats (via LNPAY.co)","description_hash":null,"payment_addr":null,"num_satoshis":100,"fee_msat":0,"expiry":86400,"expires_at":1695865627,"payment_preimage":"c5da58c5cf672aab1c05ea0fcd9eb92a2f96fc56f36178037424537ee60afe88",
    "settled":1,
    "settled_at":1695779247,"is_keysend":null,"is_amp":null,"custom_records":null
    },
    "passThru":{
        "htlc":{
            "state":"SETTLED","chanId":"883459791970238465","amtMsat":"100000","htlcIndex":"484","acceptTime":"1695779245","resolveTime":"1695779245","acceptHeight":809498,"expiryHeight":809582,"mppTotalAmtMsat":"100000"}, "type":"supersats","chat_id":35, "message":"Mais%20SuperChats!", "user_id":0,"socket_id":"ezNyoPNjvaIzMBdaAABK","wallet_id":"wal_7ihvT2Bnkq6UYi"
            }
            },

 {"id":"wtx_4UsStytEWYI5DWDyuLWp2KP","created_at":1695774118,
 "num_satoshis":100,
 "user_label":"supersats (via LNPAY.co)",
 "wal":
 {
    "id":"wal_7ihvT2Bnkq6UYi","created_at":1689884348,"updated_at":1695779247, "user_label":"SuperSats","balance":302,"balance_msat":null,"ln_node_id":"lnod_do63ikmrhktqnpj9v","default_lnurlpay_id":"lnurlp_VSifV7LMUCOOxgKn0w","statusType":{"type":"wallet","name":"active","display_name":"Active"},"walletType":{"name":"generic_wallet","display_name":"Generic Wallet"
    }
 },"wtxType":{"layer":"ln","name":"ln_deposit","display_name":"LN Deposit"},
    "lnTx":{
        "id":"lntx_ar9ZJaDCGXJdk5uJP2BYxZWz","created_at":1695774037,"ln_node_id":"lnod_do63ikmrhktqnpj9v","dest_pubkey":"0254e7f2b9d3b3e23a24e342f2eb631f7f3727c9ee1a6bea07aca792d1e1c24741","payment_request":"lnbc1u1pj3xa24pp5lrezz9q3n89r6l0yh74uucfuwvq7n30u63jf07l40mdwar82p2vqdp8wd6hqetjwdshgueq9pmxjcfqf389qs2e9e3k72gcqzzsxqyz5vqsp59uyv2er0edt4rele5lp0evz5hq96zpg83q686884wp8wq7a48f6s9qyyssqxfem0dr7e6zezhcu6gga0czmru7ln976t6gutlfmpf5w22j6ntf9s6u007tr9jkzgyvvsgrg3rcz90g5h3jgfejy8x5wwmwqqhudekgpqc6ye7","r_hash_decoded":"f8f221141199ca3d7de4bfabce613c7301e9c5fcd46497fbf57edaee8cea0a98",
 "memo":"supersats (via LNPAY.co)","description_hash":null,"payment_addr":null,"num_satoshis":100,"fee_msat":0,"expiry":86400,"expires_at":1695860437,"payment_preimage":"41f19f421f4ad5e00df3c8f727a76229c0212ca68b74c3ae1675fc12936061cc",
 "settled":1,"settled_at":1695774118,"is_keysend":null,"is_amp":null,"custom_records":null},
 "passThru":{
    "htlc":{
        "state":"SETTLED","chanId":"883459791970238465","amtMsat":"100000","htlcIndex":"483","acceptTime":"1695774118","resolveTime":"1695774118","acceptHeight":809491,"expiryHeight":809574,"mppTotalAmtMsat":"100000"},"type":"supersats","chat_id":35,"message":"Mais%20testesssss!","user_id":0,"socket_id":"16MPDZHoCO0HWgX3AAAv","wallet_id":"wal_7ihvT2Bnkq6UYi"}},{"id":"wtx_TiwNiQX38EAbMW1fG5u2sSRV","created_at":1695599693,"num_satoshis":1,"user_label":"verify supersats (via LNPAY.co)","wal":{"id":"wal_7ihvT2Bnkq6UYi","created_at":1689884348,"updated_at":1695779247,"user_label":"SuperSats","balance":302,"balance_msat":null,"ln_node_id":"lnod_do63ikmrhktqnpj9v","default_lnurlpay_id":"lnurlp_VSifV7LMUCOOxgKn0w","statusType":{"type":"wallet","name":"active","display_name":"Active"},"walletType":{"name":"generic_wallet","display_name":"Generic Wallet"}},"wtxType":{"layer":"ln","name":"ln_deposit","display_name":"LN Deposit"},"lnTx":{"id":"lntx_1ufCAaSAFsFmXtXEGDRw23nf","created_at":1695599670,"ln_node_id":"lnod_do63ikmrhktqnpj9v","dest_pubkey":"0254e7f2b9d3b3e23a24e342f2eb631f7f3727c9ee1a6bea07aca792d1e1c24741","payment_request":"lnbc10n1pj3pnpkpp5wea9lx2fmsvsscfyejnz7wr8pqt6znqrmgk84u9hzydz77apttjqdpjwejhy6tx0ys8xatsv4e8xct5wvszsanfvysycnjsg9vjucm09ycqzzsxqyz5vqsp5d4l0d4wp6m8kpzxzcd499lyehzy5tlewhhhdtkcehqh5672s0mfs9qyyssqxk2hta4749lfw8f2uwvfawedfw79azf7gzgf2mu02zpdprwx7h9nvqrja8yrjdh9cewcpzqfen23xf9qgt698f6cdudqrldscylmvwqqxc5w3q","r_hash_decoded":"767a5f9949dc19086124cca62f38670817a14c03da2c7af0b7111a2f7ba15ae4","memo":"verify supersats (via LNPAY.co)","description_hash":null,"payment_addr":null,"num_satoshis":1,"fee_msat":0,"expiry":86400,"expires_at":1695686070,"payment_preimage":"2f257b1383a5227f9b859040c9090f132d560236dffc3224e821369e1b854c06","settled":1,"settled_at":1695599693,"is_keysend":null,"is_amp":null,"custom_records":null},"passThru":{"htlc":{"state":"SETTLED","chanId":"883459791970238465","amtMsat":"1000","htlcIndex":"460","acceptTime":"1695599693","resolveTime":"1695599693","acceptHeight":809208,"expiryHeight":809291,"mppTotalAmtMsat":"1000"},"type":"verify","user_id":243,"socket_id":"UmtrnepLXZCAeBHzAAC7","wallet_id":"wal_7ihvT2Bnkq6UYi"}},{"id":"wtx_2V2tWlwV258D1FeKG4A5adI","created_at":1694740777,"num_satoshis":100,"user_label":"supersats (via LNPAY.co)","wal":{"id":"wal_7ihvT2Bnkq6UYi","created_at":1689884348,"updated_at":1695779247,"user_label":"SuperSats","balance":302,"balance_msat":null,"ln_node_id":"lnod_do63ikmrhktqnpj9v","default_lnurlpay_id":"lnurlp_VSifV7LMUCOOxgKn0w","statusType":{"type":"wallet","name":"active","display_name":"Active"},"walletType":{"name":"generic_wallet","display_name":"Generic Wallet"}},"wtxType":{"layer":"ln","name":"ln_deposit","display_name":"LN Deposit"},"lnTx":{"id":"lntx_Jj2z2pUphbGlL5MwiBDBHKSa","created_at":1694740759,"ln_node_id":"lnod_do63ikmrhktqnpj9v","dest_pubkey":"0254e7f2b9d3b3e23a24e342f2eb631f7f3727c9ee1a6bea07aca792d1e1c24741","payment_request":"lnbc1u1pjs8vghpp5x8aecthtvj7nq9tcs5w0cfllsxekvepmvdnmdz4kmva5dge0pg5qdp8wd6hqetjwdshgueq9pmxjcfqf389qs2e9e3k72gcqzzsxqyz5vqsp54txdwzmd68tgs6w5te6yftnthyjkrfyck4dyr7qj306zu6qrf2us9qyyssqhlgnm7vvv0el4gtz5r3d9ftxe7z7t0elcqgq2vyfpl27p2fecvgyayy89c89wzshedrs4ed59h94kjs9pygu7t5l6y9yp706g0h6aygqn3za55","r_hash_decoded":"31fb9c2eeb64bd301578851cfc27ff81b366643b6367b68ab6db3b46a32f0a28","memo":"supersats (via LNPAY.co)","description_hash":null,"payment_addr":null,"num_satoshis":100,"fee_msat":0,"expiry":86400,"expires_at":1694827159,"payment_preimage":"4e9214bcab363a023ab7ec8a30d02e7b9e52a67e8bfa33199ff6f055cdc95b12","settled":1,"settled_at":1694740777,"is_keysend":null,"is_amp":null,"custom_records":null},"passThru":{"htlc":{"state":"SETTLED","chanId":"883459791970238465","amtMsat":"100000","htlcIndex":"268","acceptTime":"1694740776","resolveTime":"1694740776","acceptHeight":807716,"expiryHeight":807799,"mppTotalAmtMsat":"100000"},"type":"supersats","chat_id":35,"message":"Teste%20de%20supersats%20webhook","user_id":243,"socket_id":"nBEe3D_r-7KMQwAXAAEK","wallet_id":"wal_7ihvT2Bnkq6UYi"}},{"id":"wtx_wl4RBGzTLwjxGRVT1Wcf2ud","created_at":1694740690,"num_satoshis":1,"user_label":"verify supersats (via LNPAY.co)","wal":{"id":"wal_7ihvT2Bnkq6UYi","created_at":1689884348,"updated_at":1695779247,"user_label":"SuperSats","balance":302,"balance_msat":null,"ln_node_id":"lnod_do63ikmrhktqnpj9v","default_lnurlpay_id":"lnurlp_VSifV7LMUCOOxgKn0w","statusType":{"type":"wallet","name":"active","display_name":"Active"},"walletType":{"name":"generic_wallet","display_name":"Generic Wallet"}},"wtxType":{"layer":"ln","name":"ln_deposit","display_name":"LN Deposit"},"lnTx":{"id":"lntx_O6pxlyrdcqMb7GlY1KjrJSKz","created_at":1694740661,"ln_node_id":"lnod_do63ikmrhktqnpj9v","dest_pubkey":"0254e7f2b9d3b3e23a24e342f2eb631f7f3727c9ee1a6bea07aca792d1e1c24741","payment_request":"lnbc10n1pjs8v94pp54eypaggezdnw27r5ys8mdze9er9c43f8q6qp7n3ppgtp9jqxpwzqdpjwejhy6tx0ys8xatsv4e8xct5wvszsanfvysycnjsg9vjucm09ycqzzsxqyz5vqsp5rwjl4wkdlc0tzmr64jz6el3nj5nphcvlg4al2cq2a25yfcyw4xpq9qyyssqdnlfxj8vk04mrcv9s54ge0h79twhw8xe3zlchzut7rfgjxytams8m53xnmtnpme7cwz5swd7ss0dptdqctmmppu4ntwlw70r7z38drqqrvcvuj","r_hash_decoded":"ae481ea1191366e57874240fb68b25c8cb8ac52706801f4e210a1612c8060b84","memo":"verify supersats (via LNPAY.co)","description_hash":null,"payment_addr":null,"num_satoshis":1,"fee_msat":0,"expiry":86400,"expires_at":1694827061,"payment_preimage":"c81c8aa8bea42442d199bdf558713b21c9ed57ed97fdb3dd5c967c2af7e02ec4","settled":1,"settled_at":1694740690,"is_keysend":null,"is_amp":null,"custom_records":null},"passThru":{"htlc":{"state":"SETTLED","chanId":"883459791970238465","amtMsat":"1000","htlcIndex":"267","acceptTime":"1694740689","resolveTime":"1694740689","acceptHeight":807716,"expiryHeight":807799,"mppTotalAmtMsat":"1000"},"type":"verify","user_id":243,"socket_id":"rvbA9QcX3PL8-08vAADu","wallet_id":"wal_7ihvT2Bnkq6UYi"}}]