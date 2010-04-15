#include "matrix_test.hpp"

#include <vmmlib/matrix.hpp>
#include <vmmlib/vector.hpp>

#include <sstream>

namespace vmml
{

bool
matrix_test::run()
{
    bool ok = true;
    
    matrix< 2, 3, double > m0;
    double data[] = { 1, 2, 3, 4, 5, 6 };
    
    m0.set( data, data + 6 );
    
    // test operator== / operator !=
    {
        bool ok = true;
		matrix< 2, 3, double > m0_copy;
		m0.set( data, data + 6 );
		m0_copy.set( data, data + 6 );
		ok = m0 == m0_copy;
		if ( ok )
		{
			ok = ! ( m0 != m0_copy );
		}
		log( "operator==, operator!=", ok );
		if ( ! ok )
		{
			std::stringstream error;
			error << m0 << m0_copy << std::endl;
			log_error( error.str() );
		}
	}

    // test equal
    {
        bool ok = true;
		matrix< 2, 3, double > m0_copy;
		m0.set( data, data + 6 );
		m0_copy.set( data, data + 6 );
        m0( 0, 0 ) += 0.00000005;
        bool oper_ok = m0 == m0_copy;
        bool eq_ok = m0.equals( m0_copy, 0.0000001 );
		if ( ! oper_ok && eq_ok )
            ok = true;
        else 
            ok = false;
		log( "equals( ..., tolerance )", ok );
		if ( ! ok )
		{
			std::stringstream error;
			error << m0 << m0_copy << std::endl;
			log_error( error.str() );
		}
	}

    // test copy ctor
    {
        bool ok = true;
		matrix< 2, 3, double > m0_copy( m0 );
		ok = m0 == m0_copy;
		if ( ok )
		{
			ok = ! ( m0 != m0_copy );
		}
		log( "copy constructor", ok );
		if ( ! ok )
		{
			std::stringstream error;
			error << m0 << m0_copy << std::endl;
			log_error( error.str() );
		}
	}


    // test ::IDENTITY / ::ZERO
    {
        bool ok = true;
		matrix< 5, 5, double > identity( matrix< 5, 5, double >::IDENTITY );
		matrix< 5, 2, double > zero( matrix< 5, 2, double >::ZERO );
        
        double id_data[] = { 1,0,0,0,0, 0,1,0,0,0,  0,0,1,0,0, 0,0,0,1,0, 0,0,0,0,1 };
		matrix< 5, 5, double > id_correct;
        id_correct = id_data;
        
        double zero_data[] = { 0,0,0,0,0, 0,0,0,0,0 };
		matrix< 5, 2, double > zero_correct;
        zero_correct = zero_data;
        
		ok = identity == id_correct;
        if ( ok ) ok = zero == zero_correct;

		log( "static IDENTITY, ZERO members", ok );
		if ( ! ok )
		{
			std::stringstream error;
			error 
                << "identity "  << identity 
                << "zero "      << zero
                << std::endl;
			log_error( error.str() );
		}
	}

    // test operator+, operator +=
    {
        bool ok = true;
        
        matrix< 2, 2, double > m0, m1;
        double m0_data[] = { 1, 2, 3, 4 };
        double m1_data[] = { 2, 3, 4, 5 };
        m0 = m0_data;
        m1 = m1_data;
        
        matrix< 2, 2, double > result;
        double result_data[] = { 3, 5, 7, 9 };
        result = result_data;
        
        ok = result == m0 + m1;
        
        if ( ok )
        {
            m0 = m0_data;
            m0 += m1;
            ok = result == m0;
        }
		log( "matrix addition: operator+, operator+=", ok );
	}



    // test operator+, operator +=
    {
        bool ok = true;
        
        matrix< 2, 2, double > m0, m1;
        double m0_data[] = { 3, 5, 7, 9 };
        double m1_data[] = { 2, 3, 4, 5 };
        m0 = m0_data;
        m1 = m1_data;
        
        matrix< 2, 2, double > result;
        double result_data[] = { 1, 2, 3, 4 };
        result = result_data;
        
        ok = result == m0 - m1;
        
        if ( ok )
        {
            m0 = m0_data;
            m0 -= m1;
            ok = result == m0;
        }
		log( "matrix subtraction: operator-, operator-=", ok );
	}


    // test operator[]
    {
        bool ok = true;
		m0 = data;
		ok = m0[ 1 ][ 1 ] == 5;
		if ( ok )
			ok = m0[ 1 ][ 1 ] == m0.at( 1, 1 );
		if ( ok ) 
		{
			m0[ 1 ][ 2 ] = 23;
			ok = m0.at( 1, 2 ) == 23;
		}
		
		log( "operator[]", ok );
		if ( ! ok )
		{
			std::stringstream error;
			error 
				<< " m0 " << m0 
				<< " m0[ 1 ][ 1 ] " << m0[ 1 ][ 1 ] 
				<< " m0.at( 1, 1 ) " << m0.at( 1, 1 )
				<< std::endl;
			log_error( error.str() );
		}
	}

    // test getRow/setRow/getColumn/setColumn
    {
        bool ok = true;
		matrix< 2, 3, double > M;
		double Mdata[] = { 1, 2, 3, 4, 5, 6 };
		M = Mdata;
		matrix< 1, 3, double > M_row;
        M.get_row( 1, M_row );
        
		matrix< 2, 1, double > M_column;
        M.get_column( 2, M_column );
		
		for( size_t column = 0; ok && column < 3; ++column )
		{
			ok = M.at( 1, column ) == M_row.at( 0, column );
		}

		for( size_t row = 0; ok && row < 2; ++row )
		{
			ok = M.at( row, 2 ) == M_column.at( row, 0 );
		}
		
		double Mdata_row[] = { 3, 2, 5, 4, 5, 6 };
		matrix< 2, 3, double > Mr;
		Mr = Mdata_row;
		
		M = Mdata;
		M_row.at( 0, 0 ) = 3;
		M_row.at( 0, 1 ) = 2;
		M_row.at( 0, 2 ) = 5;
		M.set_row( 0, M_row );
		for( size_t column = 0; ok && column < 3; ++column )
		{
			ok = M == Mr;
		}
		
		double Mdata_column[] = { 1, 5, 3, 4, 2, 6 };
		matrix< 2, 3, double > Mc;
		Mc = Mdata_column;
		M = Mdata;
		M_column.at( 0, 0 ) = 5;
		M_column.at( 1, 0 ) = 2;
		M.set_column( 1, M_column );
		for( size_t row = 0; ok && row < 2; ++row )
		{
			ok = M == Mc;
		}

		log( "get/set_row(),get/set_column()", ok );
		if ( ! ok )
		{
			std::stringstream error;
			error 
				<< "M " << M 
				<< "M_row " << M_row 
				<< "Mr " << Mr 
				<< "M_column " << M_column 
				<< "Mc " << Mc 
				<< std::endl;
			log_error( error.str() );
		}
	}

	
    // test transpose functionality 
    {
        bool ok = true;
		m0 = data;
        matrix< 3, 2, double > m1;
        matrix< 3, 2, double > m0t = transpose( m0 );
        m1.set( data, data + 6, false );
        
        ok = m1 == m0t;
		log( "transpose(), transpose_to()", ok );
        if ( !ok )
        {
			std::stringstream error;
			error << m1 << m0t << std::endl;
			log_error( error.str() );
		}
    }
    
    
    // test multiplication
    {
        bool ok = true;
        matrix< 2, 3, double > mul0;
        double mul0data[] = { 1, 0, 2, -1, 3, 1 };
        mul0 = mul0data;
        
        matrix< 3, 2, double > mul1;
        double mul1data[] = { 3, 1, 2, 1, 1, 0 };
        mul1 = mul1data;
        
        matrix< 2, 2, double > result;
        result.multiply( mul0, mul1 );
        
        matrix< 2, 2, double > correct_result;
        double correct_result_data[] = { 5, 1, 4, 2 };
        correct_result = correct_result_data;
        ok = result == correct_result;
    
        if ( ok )
        {
            result = mul0 * mul1;
            ok = result == correct_result;
        }
        
        if ( ok )
        {
            matrix< 2, 2, double > id2;
            identity( id2 );
            
            if ( result * id2 != result )
            {
                ok = false;
            }
        }

        if ( ok )
        {
            #if 1
            matrix< 2, 2, double > mul0, mul1;
            matrix< 2, 2, double > correct_result;
            double res_data[] = { 3, 1, 4, 1 };
            correct_result = res_data;
            #endif
            mul0.set( mul0data, mul0data + 4 );
            mul1.set( mul1data, mul1data + 4 );
            mul0 *= mul1;
            ok = mul0 == correct_result;
        }

		log( "matrix multiplication ( multiply(), operator*, operator*= )", ok );

        if ( ! ok )
        {
			std::stringstream error;
			error
				<< "result = M0 * M1 \n"
                << "M0 "        << mul0 
                << "M1 "        << mul1 
                << "result "    << result
                << std::endl;
			log_error( error.str() );
        }
    }

    // test matrix * column vector multiplication
    {
        bool ok = true;
        matrix< 4, 4, double > transform;
        double transformData[] = 
        {
            0.6555, 0.2769, 0.6948, 0.4387, 
            0.1712, 0.0462, 0.3171, 0.3816, 
            0.7060, 0.0971, 0.9502, 0.7655, 
            0, 0, 0, 1
        };
        transform = transformData;
        vector< 4, double > v;
        double vData[] = { 0.1869, 0.4898, 0.4456, 1 };
        v = vData;
        
        vector< 4, double > v_result;
        v_result = transform * v;
        
        vector< 4, double > v_correct_result;
        double vResultData[] = { 1.0064414500000000707302660885034, 
            .57752579999999997806270357614267, 
            1.3684200999999998060729922144674, 
            1. };
        v_correct_result = vResultData;
        
        ok = v_result == v_correct_result;
        
        log( "matrix * vector multiplication", ok );
        if ( ! ok )
        {
            std::stringstream ss;
            ss 
                << "A " << transform 
                << "v              " << v << "\n"
                << "v_result       " << v_result << "\n"
                << "correct result " << v_correct_result << "\n"
                << std::endl;
            log_error( ss.str() );
        }
        
    }



    // test matrix4x4 * vector3 multiplication
    {
        bool ok = true;
        matrix< 4, 4, double > transform;
        double transformData[] = 
        {
            0.6555, 0.2769, 0.6948, 0.4387, 
            0.1712, 0.0462, 0.3171, 0.3816, 
            0.7060, 0.0971, 0.9502, 0.7655, 
            0, 0, 0, 1
        };
        transform = transformData;
        vector< 3, double > v;
        double vData[] = { 0.1869, 0.4898, 0.4456 };
        v = vData;
        
        vector< 3, double > v_result;
        v_result = transform * v;
        
        vector< 3, double > v_correct_result;
        double vResultData[] =
        {   
            1.0064414500000000707302660885034, 
            .57752579999999997806270357614267, 
            1.3684200999999998060729922144674, 
            1. 
        };
        v_correct_result = vResultData;
        
        ok = v_result.equals( v_correct_result );
        
        log( "matrix4x4 * vector3 ( m4x4 * v4( v3.xyz, 1.0 ) ) multiplication", ok );
        if ( ! ok )
        {
            std::stringstream ss;
            ss 
                << "A " << transform 
                << "v              " << v << "\n"
                << "v_result       " << v_result << "\n"
                << "correct result " << v_correct_result << "\n"
                << "diff " << v_result - v_correct_result << "\n"
                << std::endl;
            log_error( ss.str() );
        }
        
    }



	#ifdef VMMLIB_SAFE_ACCESSORS
	ok = true;
	{
		matrix< 3, 2 > m;
		try
		{
			m.at( 3, 2 );
			ok = false;
		}
		catch(...)
		{}
		try
		{
			if ( ok ) m.at( 1, 1 );
		}
		catch(...)
		{
			ok = false;
		}
		try
		{
			if ( ok ) 
			{
				m[ 3 ][ 2 ];
				ok = false;
			}
		}
		catch(...)
		{}
		log( "safe accessors (debug option)", ok );
	}
	#endif

	// getSubMatrix
    {
        matrix< 2, 3, double >  m_src;
        double m_src_data[] = { 1, 2, 3, 4, 5, 6 };
        m_src = m_src_data;
        matrix< 1, 2, double >  m_sub;

        m_src.get_sub_matrix( m_sub, 1, 1 );
        
        matrix< 1, 2, double > result;
        double res_data[] = { 5, 6 };
        result = res_data;
        
        bool ok = m_sub == result;
        log( "get_sub_matrix()", ok );
	}
    
    {
        matrix< 2, 3, double >  m_src;
        double m_src_data[] = { 1, 2, 3, 4, 5, 6 };
        m_src = m_src_data;
        
        matrix< 2, 2, double >  m_sub;
        double m_sub_data[] = { 7, 8, 9, 0 };
        m_sub = m_sub_data;
        
        m_src.set_sub_matrix( m_sub, 0, 1 );
        
        matrix< 2, 3, double > result;
        double res_data[] = { 1, 7, 8, 4, 9, 0 };
        result = res_data;
        
        bool ok = m_src == result;
        log( "set_sub_matrix()", ok );
    
    }


    // matrix inversion for 2x2
    {
        bool ok = true;
        matrix< 2, 2, double > M, M_inverse, M_inverse_correct;
        double Mdata[] = 
        #if 1
        { 1., 3., 4., 2. };
        #else
        {
            .81472368639317893634910205946653, .12698681629350605515327288230765,
            .90579193707561922455084868488484, .91337585613901939307623933927971
        };
        #endif
        M = Mdata;

        double M_inverse_correct_data[] = 
        #if 1
        { -0.2, 0.3, 0.4, -0.1 };
        #else
        {   1.4518186460466018239401364553487, -.20184661818884985784450236678822,
            -1.4397639425722887906999858387280,  1.2950101881184494789778227641364
        };
        #endif
        M_inverse_correct = M_inverse_correct_data;

        M.inverse( M_inverse );
               
        ok = M_inverse == M_inverse_correct;
		log( "matrix inversion for 2x2 matrices, maximum precision", ok, true );
        if ( ! ok )
        {
            ok = M_inverse.equals( M_inverse_correct, 1e-15 );
			log( "matrix inversion 2x2, tolerance: 1e-15.", ok );
        }
        if ( ! ok )
        {  
			std::stringstream error;
			error
                << "matrix M " << M 
                << "inverse (computed)" << M_inverse 
                << "inverse (correct)" << M_inverse_correct 
                << " diffs " << M_inverse - M_inverse_correct 
                << std::endl;
			log_error( error.str() );
        }
    }


    // matrix inversion for 3x3 
    {
        bool ok = true;
        matrix< 3, 3 > M, M_inverse, M_inverse_correct;
        double Mdata[] = { 8, 1, 6, 3, 5, 7, 4, 9, 2 };
        M.set( Mdata, Mdata + 9 );

        double M_inverse_correct_data[] = 
            {   .14722222222222222222222222222222, -.14444444444444444444444444444444, .63888888888888888888888888888889e-1,
                -.61111111111111111111111111111111e-1, .22222222222222222222222222222222e-1, .10555555555555555555555555555556,
                -.19444444444444444444444444444444e-1, .18888888888888888888888888888889, -.10277777777777777777777777777778 };

        M_inverse_correct.set( M_inverse_correct_data, M_inverse_correct_data + 9 );

        M.inverse( M_inverse );
        
        ok = M_inverse == M_inverse_correct;
		log( "matrix inversion for 3x3 matrices, maximum precision", ok, true );
        if ( ! ok )
        {
            ok = M_inverse.equals( M_inverse_correct, 1e-15 );
			log( "matrix inversion 3x3, tolerance: 1e-15.", ok );
        }
        if ( ! ok )
        {  
			std::stringstream error;
			error
                << "matrix M " << M 
                << "inverse (computed)" << M_inverse 
                << "inverse (correct)" << M_inverse_correct 
                << std::endl;
			log_error( error.str() );
        }
    }


    // matrix inversion for 4x4 
    {
        bool ok = true;
        matrix< 4, 4, double > M, M_inverse, M_inverse_correct;
        double Mdata[] = { 17., 24., 1., 8., 23., 5., 7., 14.,
             4., 6., 13., 20., 10., 12., 19., 21. };
        M.set( Mdata, Mdata + 16 );

        double M_inverse_correct_data[] = { -5.780346820809248e-03, 4.962205424633170e-02, -4.811027123165852e-02, 1.493997332147622e-02, 
                4.277456647398844e-02, -3.797243219208537e-02, -1.013783903957314e-02, 1.867496665184526e-02, 
                -3.930635838150288e-02, -1.333926189417519e-02, -1.333036905291240e-01, 1.508225878168074e-01, 
                1.387283236994219e-02, 1.013783903957314e-02, 1.493108048021343e-01, -1.066251667407737e-01 };

        M_inverse_correct.set( M_inverse_correct_data, M_inverse_correct_data + 16 );

        M.inverse( M_inverse );
        
        ok = M_inverse == M_inverse_correct;
		log( "matrix inversion for 4x4 matrices, maximum precision", ok, true );
        if ( ! ok )
        {
            ok = M_inverse.equals( M_inverse_correct, 1e-9 );
			log( "matrix inversion 4x4, tolerance: 1e-15.", ok );
        }
        if ( ! ok )
        {  
			std::stringstream error;
			error
                << "matrix M " << M 
                << "inverse (computed)" << M_inverse 
                << "inverse (correct)" << M_inverse_correct 
                << "diff " << M_inverse - M_inverse_correct
                << std::endl;
			log_error( error.str() );
        }
    }
    
    // set( .... )
    {
        bool ok = true;
        
        double mData[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        
        matrix< 4, 4, double > m4x4;
        matrix< 4, 4, double > m4x4C;
        m4x4C = mData;

        m4x4.set( mData, mData + 16, true );
        
        ok = m4x4 == m4x4C;
        
        m4x4C = transpose( m4x4C );
        
        m4x4.set( mData, mData + 16, false );
        
        ok = m4x4 == m4x4C;
        
        log( "set( iterator_t, bool ) ", ok );
    
    }
    
    // test direct sum
    {
        matrix< 2, 1, double > upper_left;
        double ul_data[] = { 1, 2 };
        upper_left = ul_data;
        
        matrix< 2, 3, double > lower_right;
        double lr_data[] = { 3, 4, 5, 6, 7, 8 };
        lower_right = lr_data;
        
        matrix< 4, 4, double > result;
        matrix< 4, 4, double > correct_result;
        double corr_res_data[] = {
            1, 0, 0, 0,
            2, 0, 0, 0, 
            0, 3, 4, 5, 
            0, 6, 7, 8 };
        correct_result = corr_res_data;
        
        result.direct_sum( upper_left, lower_right );
        bool ok = result == correct_result;
        log( "direct_sum", ok );
    }
    
    
    {
        bool ok = true;
        matrix< 4, 4, double > m;
        identity( m );
        
        vector< 3, double >  v;
        double scale[] = { 1.0, 0.0, 0.0 };
        m.pre_rotate_x( 2.3 );
        //m.scale( scale );
        
        #if 0
        if ( is_positive_definite( m ) )
        {
        
        }
        #endif
        
        std::string data;
        //m.getString( data );
        //std::cout << data << std::endl;
    }
    return ok;
}




} // namespace vmml
