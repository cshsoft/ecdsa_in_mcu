	AREA LOCAL_FILE_AREA,DATA,READONLY  
	
	;public key
	EXPORT	PUBLICKEY_CART
PUBLICKEY_CART
	incbin	publickey.cert
	
	;private key
	EXPORT	PRIVATE_CART
PRIVATE_CART		
	incbin	private.cert
		
	;sign cart for chip
	EXPORT	SIGN_CART
SIGN_CART
	incbin	sign_hash.sig
	
	;cpu uid for test
	EXPORT	CPU_ID
CPU_ID	
	incbin	cpu.uid
	
	END
	