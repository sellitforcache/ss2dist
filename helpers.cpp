/* 
function to decode the bitarray
*/

void decode_bitarray(unsigned bitarray, unsigned* ipt, unsigned* i_positron, unsigned* jgp){
	// HEAVY IONS NOT IMPLEMENTED!
	// encoding positions
	unsigned pack_jgp   =        2; //                          2
	unsigned pack_i_pos =        4; //                      2 * 2
	unsigned pack_ipt   =        8; //                  2 * 2 * 2
	unsigned pack_ion_a =      512; //             64 * 2 * 2 * 2
	unsigned pack_ion_z =   262144; //       512 * 64 * 2 * 2 * 2
	unsigned pack_ion_s = 33554432; // 128 * 512 * 64 * 2 * 2 * 2

	unsigned b = bitarray;
	unsigned f;

	f = b%pack_ipt;
	ipt[0]        = (b-f)/pack_ipt;
	b=f;

	f = b%pack_i_pos;
	i_positron[0] = (b-f)/pack_i_pos;
	b=f;

	f = b%pack_jgp;
	jgp[0]        = (b-f)/pack_jgp;

}