#ifndef __AUX_BH_OPT_H__
#define __AUX_BH_OPT_H__

#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>
#include <cstdlib>

#ifndef VAL_TYPE
#define VAL_TYPE int
#endif

#ifndef INF
#define INF INT_MAX
#endif

typedef VAL_TYPE static_data_type;

class AuxBufferHeap
{
	private:
		enum buff_ids { _B_, _U_ };
		enum misc { NIL = -1 };
		int QUICK_BASE;
		static_data_type* data;
		int r, top, nu_0;
		int data_size;
		VAL_TYPE min_data[ 4096 ];
		VAL_TYPE new_ins_data[ 256 ];
		int mn, mn_st;
		int MAX_MIN_DATA;
		int new_ins;
		int MAX_NEW_INS_2;
		void init_vars(void);
		void basic_init(int init_alloc);
		void resize_data(int new_size);
		void insertion_sort(VAL_TYPE* A, int p, int r);
		void quick_sort(static_data_type* A, int p, int r);
		void get_next_update(int& ii, VAL_TYPE& ui, VAL_TYPE& uk);
		void two_way_get_next_update(int& ia, int& ib, VAL_TYPE& ui, VAL_TYPE& uk);
		void three_way_get_next_update(int& ia, int& ib, int& ic, VAL_TYPE& ui, VAL_TYPE& uk);
		void save_elements_and_update(int& id, int jd, int& i, int& k, VAL_TYPE ui, VAL_TYPE uk);
		void save_elements_and_discard_update(int& id, int jd, int& k, VAL_TYPE ui, VAL_TYPE uk);
		void three_way_U_merge(int ia, int ja, int ib, int jb, int ic, int jc, int& i);
		void three_way_U_merge_and_apply(int ia, int ja, int ib, int jb, int ic, int jc, int id, int jd, int& i, int& k);
		void find_buffers(int& ia, int& ja, int& ib, int& jb, int& id, int& jd, int& nu, int& nb, int& l);
		void redistribute(int bi, int bj, int l, int& mx, VAL_TYPE& mk);
		void update_mini_buffer(void);
		void print_Aux_BH(char* s);

	public:
		AuxBufferHeap();
		AuxBufferHeap(int init_alloc);
		~AuxBufferHeap();
		void Insert(int x, VAL_TYPE key);
		void Delete_Min(int& mx, VAL_TYPE& mk);
		void Find_Min(int& mx, VAL_TYPE& mk);
};

#endif
