#include "random.h"

static HCRYPTPROV g_hProv;

/*++

	Initialize the random generator, using a Win32 API call.
	This function must be called before any call to GenerateRandomNumber().

--*/

VOID InitializeRandomGenerator(VOID)
{
	CryptAcquireContext(&g_hProv, 
						NULL,
						NULL,
						PROV_RSA_FULL,
						CRYPT_VERIFYCONTEXT);
}

/*++

	Release the random generator.

--*/

VOID ReleaseRandomGenerator(VOID)
{
	CryptReleaseContext(g_hProv, 0);
}

/*++

	This function returns a random number, based on the generator initialized
	before, between 0 and nMax

--*/

UINT GenerateRandomNumber (UINT nMax)
{
    UINT RandomNumber = 0;
    UINT Buckets = UINT_MAX / (nMax + 1);
    UINT Limit = Buckets * (nMax + 1);
    do
    {
		if (!CryptGenRandom(g_hProv, sizeof(UINT), (PBYTE) &RandomNumber))
        {
            return 0U;
        }
    } while(RandomNumber >= Limit);
    return RandomNumber / Buckets;
}
