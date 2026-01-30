#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <Docking/AuxBH.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

void AuxBufferHeap::init_vars(void)
{
	r = 1;
	top = 0;
	nu_0 = 0;
	mn = 0;
	mn_st = 2;
	min_data[ 1 ] = -INF;
	new_ins = 2;
}

void AuxBufferHeap::basic_init(int init_alloc)
{
	init_vars();
	QUICK_BASE = 35;
	MAX_MIN_DATA = 256;
	MAX_NEW_INS_2 = 26;   // always keep: MAX_NEW_INS_2 <= 2 * MAX_MIN_DATA
	data = new static_data_type[ init_alloc ];
	data_size = init_alloc;
	srand48(time(NULL));
}

AuxBufferHeap::AuxBufferHeap()
{
	basic_init(128);
}

AuxBufferHeap::AuxBufferHeap(int init_alloc)
{
	basic_init(init_alloc);
}

AuxBufferHeap::~AuxBufferHeap()
{
	delete [] data;
}

inline void AuxBufferHeap::resize_data(int new_size)
{
	static_data_type* tmp;
	int old_size = data_size;
	while (new_size > data_size)
	{
		data_size <<= 1;
	}
	tmp = new static_data_type[ data_size ];
	memcpy(tmp, data, old_size * sizeof(data[ 0 ]));
	delete [] data;
	data = tmp;
}

inline void AuxBufferHeap::insertion_sort(VAL_TYPE* A, int p, int r)
{
	VAL_TYPE ti, tk;
	int j;
	A[ p - 1 ] = -INF;
	for (int i = p + 2; i <= r; i += 2)
	{
		ti = A[ i ];
		tk = A[ i + 1 ];
		j = i - 2;
		while (tk < A[ j + 1 ])
		{
			A[ j + 2 ] = A[ j ];
			A[ j + 3 ] = A[ j + 1 ];
			j -= 2;
		}
		if (j != i - 2)
		{
			A[ j + 2 ] = ti;
			A[ j + 3 ] = tk;
		}
	}
}

void AuxBufferHeap::quick_sort(static_data_type* A, int p, int r)
{
	VAL_TYPE ti, tk, t;
	while (r - p >= QUICK_BASE)
	{
		int q = (lrand48() % ((int)((r - p) >> 1) + 1));
		q = p + (q << 1);
		ti = A[ q ];
		tk = A[ q + 1 ];
		A[ q ] = A[ r ];
		A[ q + 1 ] = A[ r + 1 ];
		q = p - 2;
		for (int j = p; j <= r - 2; j += 2)
			if (A[ j + 1 ] < tk)
			{
				q = q + 2;
				t = A[ q ];
				A[ q ] = A[ j ];
				A[ j ] = t;
				t = A[ q + 1 ];
				A[ q + 1 ] = A[ j + 1 ];
				A[ j + 1 ] = t;
			}
		q = q + 2;
		A[ r ] = A[ q ];
		A[ r + 1 ] = A[ q + 1 ];
		A[ q ] = ti;
		A[ q + 1 ] = tk;
		if ((q - p < r - q) && (q - p > 2))
		{
			quick_sort(A, p, q - 2);
			p = q + 2;
		}
		else
		{
			quick_sort(A, q + 2, r);
			r = q - 2;
		}
	}
	int j;
	{
		for (int i = p + 2; i <= r; i += 2)
		{
			ti = A[ i ];
			tk = A[ i + 1 ];
			j = i - 2;
			while ((j >= p) && (tk < A[ j + 1 ]))
			{
				A[ j + 2 ] = A[ j ];
				A[ j + 3 ] = A[ j + 1 ];
				j -= 2;
			}
			if (j != i - 2)
			{
				A[ j + 2 ] = ti;
				A[ j + 3 ] = tk;
			}
		}
	}
}

inline void AuxBufferHeap::get_next_update(int& ii, VAL_TYPE& ui, VAL_TYPE& uk)
{
	ui = data[ ii ];
	uk = data[ ii + 1 ];
	ii += 2;
}

inline void AuxBufferHeap::two_way_get_next_update(int& ia, int& ib, VAL_TYPE& ui, VAL_TYPE& uk)
{
	if (data[ ia + 1 ] < data[ ib + 1 ])
	{
		get_next_update(ia, ui, uk);
	}
	else
	{
		get_next_update(ib, ui, uk);
	}
}

inline void AuxBufferHeap::three_way_get_next_update(int& ia, int& ib, int& ic, VAL_TYPE& ui, VAL_TYPE& uk)
{
	if (data[ ia + 1 ] < data[ ib + 1 ])
	{
		two_way_get_next_update(ia, ic, ui, uk);
	}
	else
	{
		two_way_get_next_update(ib, ic, ui, uk);
	}
}

inline void AuxBufferHeap::save_elements_and_update(int& id, int jd, int& i, int& k, VAL_TYPE ui, VAL_TYPE uk)
{
	while ((id <= jd) && (data[ id + 1 ] <= uk))
	{
		data[ k ] = data[ id ];
		data[ k + 1 ] = data[ id + 1 ];
		id += 2;
		k += 2;
	}
	if (id <= jd)
	{
		data[ k ] = ui;
		data[ k + 1 ] = uk;
		k += 2;
	}
	else
	{
		data[ i ] = ui;
		data[ i + 1 ] = uk;
		i += 2;
	}
}

inline void AuxBufferHeap::save_elements_and_discard_update(int& id, int jd, int& k, VAL_TYPE ui, VAL_TYPE uk)
{
	while ((id <= jd) && (data[ id + 1 ] <= uk))
	{
		data[ k ] = data[ id ];
		data[ k + 1 ] = data[ id + 1 ];
		id += 2;
		k += 2;
	}
	data[ k ] = ui;
	data[ k + 1 ] = uk;
	k += 2;
}

void AuxBufferHeap::three_way_U_merge(int ia, int ja, int ib, int jb, int ic, int jc, int& i)
{
	if ((ia <= ja) && (ib <= jb) && (ic <= jc))
	{
		VAL_TYPE* f0, *f1, *f2, *to, *done;
		VAL_TYPE k0, k1, k2;
		int nn = ja - ia + jb - ib + jc - ic + 6, n0 = (ja - ia + 2) >> 1, n1 = (jb - ib + 2) >> 1, n2 = (jc - ic + 2) >> 1;
		f0 = &data[ ia ];
		f1 = &data[ ib ];
		f2 = &data[ ic ];
		to = &data[ i ];
		done = &data[ i + nn ];
		k0 = data[ ia + 1 ];
		k1 = data[ ib + 1 ];
		k2 = data[ ic + 1 ];
		if (k0 < k1)
		{
			if (k1 < k2)
			{
				goto s012;
			}
			else
			{
				if (k2 < k0)
				{
					goto s201;
				}
				else
				{
					goto s021;
				}
			}
		}
		else
		{
			if (k1 < k2)
			{
				if (k0 < k2)
				{
					goto s102;
				}
				else
				{
					goto s120;
				}
			}
			else
			{
				goto s210;
			}
		}
#define Merge3Case( a, b, c )\
        s ## a ## b ## c :\
        if ( to == done ) goto finish;\
        ( *to ) = ( *f ## a );\
        ( *( to + 1 ) ) = k ## a;\
        to += 2;\
        f ## a += 2;\
        if ( --n ## a ) k ## a = ( *( f ## a + 1 ) ); else k ## a = INF;\
        if ( k ## a < k ## b ) goto s ## a ## b ## c;\
        if ( k ## a < k ## c ) goto s ## b ## a ## c;\
        goto s ## b ## c ## a;
		Merge3Case(0, 1, 2);
		Merge3Case(1, 2, 0);
		Merge3Case(2, 0, 1);
		Merge3Case(1, 0, 2);
		Merge3Case(0, 2, 1);
		Merge3Case(2, 1, 0);
	finish:
		i += nn;
		return;
	}
	if (ia > ja)
	{
		while ((ib <= jb) && (ic <= jc))
		{
			while (data[ ib + 1 ] <= data[ ic + 1 ])
			{
				data[ i ] = data[ ib ];
				data[ i + 1 ] = data[ ib + 1 ];
				i += 2;
				ib += 2;
				if (ib > jb)
				{
					break;
				}
			}
			data[ i ] = data[ ic ];
			data[ i + 1 ] = data[ ic + 1 ];
			i += 2;
			ic += 2;
		}
	}
	else if (ib > jb)
	{
		while ((ia <= ja) && (ic <= jc))
		{
			while (data[ ia + 1 ] <= data[ ic + 1 ])
			{
				data[ i ] = data[ ia ];
				data[ i + 1 ] = data[ ia + 1 ];
				i += 2;
				ia += 2;
				if (ia > ja)
				{
					break;
				}
			}
			data[ i ] = data[ ic ];
			data[ i + 1 ] = data[ ic + 1 ];
			i += 2;
			ic += 2;
		}
	}
	else
	{
		while ((ia <= ja) && (ib <= jb))
		{
			while (data[ ia + 1 ] <= data[ ib + 1 ])
			{
				data[ i ] = data[ ia ];
				data[ i + 1 ] = data[ ia + 1 ];
				i += 2;
				ia += 2;
				if (ia > ja)
				{
					break;
				}
			}
			data[ i ] = data[ ib ];
			data[ i + 1 ] = data[ ib + 1 ];
			i += 2;
			ib += 2;
		}
	}
	if (ia <= ja)
	{
		while (ia <= ja)
		{
			data[ i ] = data[ ia ];
			data[ i + 1 ] = data[ ia + 1 ];
			i += 2;
			ia += 2;
		}
	}
	else if (ib <= jb)
	{
		while (ib <= jb)
		{
			data[ i ] = data[ ib ];
			data[ i + 1 ] = data[ ib + 1 ];
			i += 2;
			ib += 2;
		}
	}
	else
	{
		while (ic <= jc)
		{
			data[ i ] = data[ ic ];
			data[ i + 1 ] = data[ ic + 1 ];
			i += 2;
			ic += 2;
		}
	}
}

void AuxBufferHeap::three_way_U_merge_and_apply(int ia, int ja, int ib, int jb, int ic, int jc, int id, int jd, int& i, int& k)
{
	VAL_TYPE ui, uk;
	int kk = k;
	if (i != NIL)
	{
		while ((id <= jd) && (ia <= ja) && (ib <= jb) && (ic <= jc))
		{
			three_way_get_next_update(ia, ib, ic, ui, uk);
			save_elements_and_update(id, jd, i, k, ui, uk);
		}
		if (ia > ja)
		{
			while ((id <= jd) && (ib <= jb) && (ic <= jc))
			{
				two_way_get_next_update(ib, ic, ui, uk);
				save_elements_and_update(id, jd, i, k, ui, uk);
			}
		}
		else if (ib > jb)
		{
			while ((id <= jd) && (ia <= ja) && (ic <= jc))
			{
				two_way_get_next_update(ia, ic, ui, uk);
				save_elements_and_update(id, jd, i, k, ui, uk);
			}
		}
		else
		{
			while ((id <= jd) && (ia <= ja) && (ib <= jb))
			{
				two_way_get_next_update(ia, ib, ui, uk);
				save_elements_and_update(id, jd, i, k, ui, uk);
			}
		}
		if (ia <= ja)
		{
			while ((id <= jd) && (ia <= ja))
			{
				get_next_update(ia, ui, uk);
				save_elements_and_update(id, jd, i, k, ui, uk);
			}
		}
		else if (ib <= jb)
		{
			while ((id <= jd) && (ib <= jb))
			{
				get_next_update(ib, ui, uk);
				save_elements_and_update(id, jd, i, k, ui, uk);
			}
		}
		else
		{
			while ((id <= jd) && (ic <= jc))
			{
				get_next_update(ic, ui, uk);
				save_elements_and_update(id, jd, i, k, ui, uk);
			}
		}
	}
	else
	{
		while ((id <= jd) && (ia <= ja) && (ib <= jb) && (ic <= jc))
		{
			three_way_get_next_update(ia, ib, ic, ui, uk);
			save_elements_and_discard_update(id, jd, k, ui, uk);
		}
		if (ia > ja)
		{
			while ((id <= jd) && (ib <= jb) && (ic <= jc))
			{
				two_way_get_next_update(ib, ic, ui, uk);
				save_elements_and_discard_update(id, jd, k, ui, uk);
			}
		}
		else if (ib > jb)
		{
			while ((id <= jd) && (ia <= ja) && (ic <= jc))
			{
				two_way_get_next_update(ia, ic, ui, uk);
				save_elements_and_discard_update(id, jd, k, ui, uk);
			}
		}
		else
		{
			while ((id <= jd) && (ia <= ja) && (ib <= jb))
			{
				two_way_get_next_update(ia, ib, ui, uk);
				save_elements_and_discard_update(id, jd, k, ui, uk);
			}
		}
		if (ia <= ja)
		{
			while ((id <= jd) && (ia <= ja))
			{
				get_next_update(ia, ui, uk);
				save_elements_and_discard_update(id, jd, k, ui, uk);
			}
		}
		else if (ib <= jb)
		{
			while ((id <= jd) && (ib <= jb))
			{
				get_next_update(ib, ui, uk);
				save_elements_and_discard_update(id, jd, k, ui, uk);
			}
		}
		else
		{
			while ((id <= jd) && (ic <= jc))
			{
				get_next_update(ic, ui, uk);
				save_elements_and_discard_update(id, jd, k, ui, uk);
			}
		}
	}
	VAL_TYPE* dk, *di, *dj;
	dk = &data[ k ];
	di = &data[ id ];
	dj = &data[ jd ];
	while (di <= dj)
	{
		(*dk) = (*di);
		(*(dk + 1)) = (*(di + 1));
		dk += 2;
		di += 2;
	}
	if (id <= jd)
	{
		k += (jd - id) + 2;
	}
	if (i == NIL)
	{
		three_way_U_merge(ia, ja, ib, jb, ic, jc, k);
	}
	else
	{
		three_way_U_merge(ia, ja, ib, jb, ic, jc, i);
	}
}

inline void AuxBufferHeap::find_buffers(int& ia, int& ja, int& ib, int& jb, int& id, int& jd, int& nu, int& nb, int& l)
{
	if (top >= 2)
	{
		top -= 2;
		if ((((int) data[ top ]) & 1) == _U_)
		{
			l = ((int) data[ top ]) >> 1;
			nu = (int) data[ top + 1 ];
			ja = top - 2;
			ia = top - (nu << 1);
			top = ia;
			if (top >= 2)
			{
				if ((((int) data[ top - 2 ]) & 1) == _U_)
				{
					if ((((int) data[ top - 2 ]) >> 1) == l)
					{
						top -= 2;
						jb = top - 2;
						ib = top - (((int) data[ top + 1 ]) << 1);
						nu += (int) data[ top + 1 ];
						top = ib;
						if (top >= 2)
						{
							if ((((int) data[ top - 2 ]) & 1) == _B_)
							{
								top -= 2;
								l = ((int) data[ top ]) >> 1;
								nb = (int) data[ top + 1 ];
								jd = top - 2;
								id = top - (nb << 1);
								top = id;
							}
							else
							{
								id = 2;
								jd = 0;
								nb = 0;
							}
						}
						else
						{
							id = 2;
							jd = 0;
							nb = 0;
						}
					}
					else
					{
						ib = 2;
						jb = 0;
						id = 2;
						jd = 0;
						nb = 0;
					}
				}
				else
				{
					ib = 2;
					jb = 0;
					top -= 2;
					l = ((int) data[ top ]) >> 1;
					nb = (int) data[ top + 1 ];
					jd = top - 2;
					id = top - (nb << 1);
					top = id;
				}
			}
			else
			{
				ib = 2;
				jb = 0;
				id = 2;
				jd = 0;
				nb = 0;
			}
		}
		else
		{
			ia = ib = 2;
			ja = jb = 0;
			nu = 0;
			l = ((int) data[ top ]) >> 1;
			nb = (int) data[ top + 1 ];
			jd = top - 2;
			id = top - (nb << 1);
			top = id;
		}
	}
	else
	{
		ia = ib = 2;
		ja = jb = 0;
		nu = 0;
		id = 2;
		jd = 0;
		nb = 0;
	}
}

void AuxBufferHeap::redistribute(int bi, int bj, int l, int& mx, VAL_TYPE& mk)
{
	int i, j, p, q, s, k = bj - bi, nk = k >> 1;
	if (nk > 1)
	{
		mn = 0;
		mn_st = 2;
		p = (MAX_MIN_DATA << 1);
		if (p > bj - bi - 2)
		{
			p = bj - bi - 2;
		}
		if (p > 0)
		{
			mn = p >> 1;
			{
				for (i = 2, j = bi + 2; i <= p; i += 2, j += 2)
				{
					min_data[ i ] = data[ j ];
					min_data[ i + 1 ] = data[ j + 1 ];
				}
			}
			j = bi + p;
			data[ j ] = data[ bi ];
			data[ j + 1 ] = data[ bi + 1 ];
			bi = j;
			k = bj - bi;
			nk = k >> 1;
		}
	}
	if (nk > (1 << l))
	{
		q = l - 1;
		p = top + (((1 << l) + (nk - (1 << l)) + (l + 1)) << 1);
	}
	else
	{
		q = 0;
		while ((1 << q) <= nk)
		{
			q++;
		}
		q--;
		if ((1 << q) == nk)
		{
			q--;
		}
		p = top + ((nk  + (q + 1)) << 1);
	}
	if (p >= bi)
	{
		if (p + k > data_size)
		{
			resize_data(p + k);
		}
		if (k < 64)
		{
			for (i = k - 1; i >= 0 ; i--)
			{
				data[ p + i ] = data[ bi + i ];
			}
		}
		else
		{
			memmove(&data[ p ], &data[ bi ], k * sizeof(VAL_TYPE));
		}
		bi = p;
		bj = p + k;
	}
	if (nk > (1 << l))
	{
		if (l == r - 1)
		{
			r++;
		}
		{
			for (i = (1 << (l + 1)); i < (nk << 1) - 5; i += 6)
			{
				data[ top ] = data[ bi + i ];
				data[ top + 1 ] = data[ bi + i + 1 ];
				data[ top + 2 ] = data[ bi + i + 2 ];
				data[ top + 3 ] = data[ bi + i + 3 ];
				data[ top + 4 ] = data[ bi + i + 4 ];
				data[ top + 5 ] = data[ bi + i + 5 ];
				top += 6;
			}
			for (; i < (nk << 1); i += 2)
			{
				data[ top ] = data[ bi + i ];
				data[ top + 1 ] = data[ bi + i + 1 ];
				top += 2;
			}
		}
		data[ top ] = (((l + 1) << 1)) | _U_;
		data[ top + 1 ] = nk - (1 << l);
		top += 2;
		nk = (1 << l);
	}
	if (l == r - 1)
	{
		r = ((q + 1) > 1) ? (q + 1) : 1;
	}
	s = (1 << q);
	while (q >= 0)
	{
		if (q < 6)
		{
			for (i = (s << 1); i < (nk << 1) - 5; i += 6)
			{
				data[ top ] = data[ bi + i ];
				data[ top + 1 ] = data[ bi + i + 1 ];
				data[ top + 2 ] = data[ bi + i + 2 ];
				data[ top + 3 ] = data[ bi + i + 3 ];
				data[ top + 4 ] = data[ bi + i + 4 ];
				data[ top + 5 ] = data[ bi + i + 5 ];
				top += 6;
			}
			for (; i < (nk << 1); i += 2)
			{
				data[ top ] = data[ bi + i ];
				data[ top + 1 ] = data[ bi + i + 1 ];
				top += 2;
			}
		}
		else
		{
			memcpy(&data[ top ], &data[ bi + (s << 1)], ((nk << 1) - (s << 1)) * sizeof(VAL_TYPE));
			top += ((nk << 1) - (s << 1));
		}
		data[ top ] = (q << 1) | _B_;
		data[ top + 1 ] = nk - s;
		top += 2;
		nk = s;
		s >>= 1;
		q--;
	}
	mx = (int) data[ bi ];
	mk = data[ bi + 1 ];
}

void AuxBufferHeap::print_Aux_BH(char* s)
{
	int i, op, m;
	printf("\n%s:\n", s);
	i = top;
	if (nu_0 > 0)
	{
		printf("\nU( %d, %d ):\n", 0, nu_0);
		for (int j = 0; j < nu_0; j++)
		{
			i -= 2;
			printf("id = %d, key = %lf\n", (int) data[ i ], (double) data[ i + 1 ]);
		}
	}
	while (i > 0)
	{
		i -= 2;
		op = ((int) data[ i ]) & 1;
		m = (int) data[ i + 1 ];
		printf("\n%s( %d, %d ):\n", (op == _U_) ? "U" : "B", ((int) data[ i ]) >> 1 , m);
		if (op == _U_)
		{
			for (int j = 0; j < m; j++)
			{
				i -= 2;
				printf("id = %d, key = %lf\n", (int) data[ i ], (double) data[ i + 1 ]);
			}
		}
		else
		{
			for (int j = 0; j < m; j++)
			{
				i -= 2;
				printf("id = %d, key = %lf\n", (int) data[ i ], (double) data[ i + 1 ]);
			}
		}
	}
	printf("\n");
}

inline void AuxBufferHeap::update_mini_buffer(void)
{
	int ni = (new_ins - 2) >> 1;
	if (top + (ni << 1) > data_size)
	{
		resize_data(top + (ni << 1));
	}
	if (mn > 0)
	{
		VAL_TYPE tx, tk, mk;
		int i = mn_st + (mn << 1) - 2, j, k, p;
		mk = min_data[ i + 1 ];
		p = 0;
		j = 2;
		while (j < new_ins)
		{
			tx = new_ins_data[ j ];
			tk = new_ins_data[ j + 1 ];
			if (tk <= mk)
			{
				k = p;
				while (new_ins_data[ k + 1 ] > tk)
				{
					new_ins_data[ k + 2 ] = new_ins_data[ k ];
					new_ins_data[ k + 3 ] = new_ins_data[ k + 1 ];
					k -= 2;
				}
				new_ins_data[ k + 2 ] = tx;
				new_ins_data[ k + 3 ] = tk;
				p += 2;
			}
			else
			{
				data[ top ] = tx;
				data[ top + 1 ] = tk;
				top += 2;
			}
			j += 2;
		}
		k = p;
		while (k)
		{
			tk = new_ins_data[ k + 1 ];
			while (min_data[ i + 1 ] > tk)
			{
				min_data[ i + k ] = min_data[ i ];
				min_data[ i + k + 1 ] = min_data[ i + 1 ];
				i -= 2;
			}
			min_data[ i + k ] = new_ins_data[ k ];
			min_data[ i + k + 1 ] = tk;
			k -= 2;
		}
		k = p >> 1;
		nu_0 += (ni - k);
		if (mn + k > MAX_MIN_DATA)
		{
			j = (mn + k) - MAX_MIN_DATA;
			mn = MAX_MIN_DATA;
			i = mn_st + (mn << 1);
			nu_0 += j;
			while (j--)
			{
				data[ top ] = min_data[ i ];
				data[ top + 1 ] = min_data[ i + 1 ];
				top += 2;
				i += 2;
			}
		}
		else
		{
			mn += k;
		}
	}
	else
	{
		if (top == 0)            // assuming ni <= MAX_MIN_DATA
		{
			if (ni > 1)
			{
				insertion_sort(new_ins_data, 2, new_ins - 2);
			}
			mn = ni;
			mn_st = 2;
			memcpy(min_data + 2, new_ins_data + 2, (ni << 1) * sizeof(VAL_TYPE));
		}
		else
		{
			memcpy(&data[ top ], new_ins_data + 2, (ni << 1) * sizeof(VAL_TYPE));
			top += (ni << 1);
			nu_0 += ni;
		}
	}
	new_ins = 2;
}

void AuxBufferHeap::Insert(int x, VAL_TYPE key)
{
	new_ins_data[ new_ins ] = x;
	new_ins_data[ new_ins + 1 ] = key;
	new_ins += 2;
	if (new_ins > MAX_NEW_INS_2)
	{
		update_mini_buffer();
	}
}

void AuxBufferHeap::Delete_Min(int& mx, VAL_TYPE& mk)
{
	int l, nu, nb;
	int ia, ja, ib, jb, ic, jc, id, jd, i, ip, k, kp, st, usz, bsz;
	if (new_ins > 2)
	{
		update_mini_buffer();
	}
	if (mn > 0)
	{
		mx = (int) min_data[ mn_st ];
		mk = min_data[ mn_st + 1 ];
		min_data[ mn_st + 1 ] = -INF;
		mn_st += 2;
		mn--;
		if (mn_st > (MAX_MIN_DATA << 2))
		{
			for (l = 0; l < (mn << 1); l++)
			{
				min_data[ 2 + l ] = min_data[ mn_st + l ];
			}
			mn_st = 2;
		}
		return;
	}
	if (top == 0)
	{
		init_vars();
		mx = NIL;
		mk = INF;
		return;
	}
	ic = top - (nu_0 << 1);
	jc = top - 2;
	if (nu_0 > 1)
	{
		quick_sort(data, ic, jc);
	}
	l = 0;
	st = top;
	top = ic;
	find_buffers(ia, ja, ib, jb, id, jd, nu, nb, l);
	nu += nu_0;
	if (l < r - 1)
	{
		usz = (nu << 1);
	}
	else
	{
		usz = 0;
	}
	if ((nb > 0) || (l == r - 1))
	{
		bsz = ((nu + nb) << 1);
	}
	else
	{
		bsz = 0;
	}
	if (st + usz + bsz > data_size)
	{
		resize_data(st + usz + bsz);
	}
	ip = i = st;
	kp = k = i + usz;
	if ((nb > 0) || (l == r - 1))
	{
		if (l == r - 1)
		{
			ip = i = NIL;
		}
		three_way_U_merge_and_apply(ia, ja, ib, jb, ic, jc, id, jd, i, k);
	}
	else
	{
		three_way_U_merge(ia, ja, ib, jb, ic, jc, i);
	}
	while ((l < r) && (k == kp))
	{
		nu_0 = (i - ip) >> 1;
		st = top;
		ic = ip;
		jc = i - 2;
		find_buffers(ia, ja, ib, jb, id, jd, nu, nb, l);
		nu += nu_0;
		if (l < r - 1)
		{
			usz = (nu << 1);
		}
		else
		{
			usz = 0;
		}
		if ((nb > 0) || (l == r - 1))
		{
			bsz = ((nu + nb) << 1);
		}
		else
		{
			bsz = 0;
		}
		if (st + usz + bsz < ip)
		{
			ip = i = st;
		}
		else
		{
			if (i + usz + bsz > data_size)
			{
				resize_data(i + usz + bsz);
			}
			ip = i;
		}
		kp = k = i + usz;
		if ((nb > 0) || (l == r - 1))
		{
			if (l == r - 1)
			{
				ip = i = NIL;
			}
			three_way_U_merge_and_apply(ia, ja, ib, jb, ic, jc, id, jd, i, k);
		}
		else
		{
			three_way_U_merge(ia, ja, ib, jb, ic, jc, i);
		}
	}
	if (i > ip)
	{
		nu_0 = (i - ip) >> 1;
		{
			memcpy(&data[ top ], &data[ ip ], (i - ip) * sizeof(VAL_TYPE));
			top += (i - ip);
			ip = i;
		}
		data[ top ] = ((l + 1) << 1) | _U_;
		data[ top + 1 ] = nu_0;
		top += 2;
	}
	if (k > kp)
	{
		redistribute(kp, k, l, mx, mk);
	}
	else
	{
		init_vars();
		mx = NIL;
		mk = INF;
	}
	nu_0 = 0;
}

void AuxBufferHeap::Find_Min(int& mx, VAL_TYPE& mk)
{
	Delete_Min(mx, mk);
	if (mk != INF)
	{
		Insert(mx, mk);
	}
}
