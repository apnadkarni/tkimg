#ifdef WIN32
/*
 * While the definitions in the original header are technically the
 * same as the definitions in 'zlibDecls.h' on Windows they are not,
 * in a small detail: DLLEXPORT vs. DLLIMPORT. As our artificial
 * definitions do the right thing we use the macro definitions below
 * to move the original definitions out of the way. We do retain the
 * inclusion of all the type definitions, etc.
 */
#define png_access_version_number __mask_png_access_version_number
#define png_set_sig_bytes __mask_png_set_sig_bytes
#define png_sig_cmp __mask_png_sig_cmp
#define png_create_read_struct __mask_png_create_read_struct
#define png_create_write_struct __mask_png_create_write_struct
#define png_get_compression_buffer_size __mask_png_get_compression_buffer_size
#define png_set_compression_buffer_size __mask_png_set_compression_buffer_size
#define png_reset_zstream __mask_png_reset_zstream
#define png_create_read_struct_2 __mask_png_create_read_struct_2
#define png_create_write_struct_2 __mask_png_create_write_struct_2
#define png_write_chunk __mask_png_write_chunk
#define png_write_chunk_start __mask_png_write_chunk_start
#define png_write_chunk_data __mask_png_write_chunk_data
#define png_write_chunk_end __mask_png_write_chunk_end
#define png_create_info_struct __mask_png_create_info_struct
#define png_info_init_3 __mask_png_info_init_3
#define png_write_info_before_PLTE __mask_png_write_info_before_PLTE
#define png_write_info __mask_png_write_info
#define png_read_info __mask_png_read_info
#define png_convert_to_rfc1123 __mask_png_convert_to_rfc1123
#define png_convert_from_struct_tm __mask_png_convert_from_struct_tm
#define png_convert_from_time_t __mask_png_convert_from_time_t
#define png_set_expand __mask_png_set_expand
#define png_set_palette_to_rgb __mask_png_set_palette_to_rgb
#define png_set_tRNS_to_alpha __mask_png_set_tRNS_to_alpha
#define png_set_bgr __mask_png_set_bgr
#define png_set_gray_to_rgb __mask_png_set_gray_to_rgb
#define png_set_rgb_to_gray __mask_png_set_rgb_to_gray
#define png_set_rgb_to_gray_fixed __mask_png_set_rgb_to_gray_fixed
#define png_get_rgb_to_gray_status __mask_png_get_rgb_to_gray_status
#define png_build_grayscale_palette __mask_png_build_grayscale_palette
#define png_set_strip_alpha __mask_png_set_strip_alpha
#define png_set_swap_alpha __mask_png_set_swap_alpha
#define png_set_invert_alpha __mask_png_set_invert_alpha
#define png_set_filler __mask_png_set_filler
#define png_set_swap __mask_png_set_swap
#define png_set_packing __mask_png_set_packing
#define png_set_packswap __mask_png_set_packswap
#define png_set_shift __mask_png_set_shift
#define png_set_interlace_handling __mask_png_set_interlace_handling
#define png_set_invert_mono __mask_png_set_invert_mono
#define png_set_background __mask_png_set_background
#define png_set_strip_16 __mask_png_set_strip_16
#define png_set_dither __mask_png_set_dither
#define png_set_gamma __mask_png_set_gamma
#define png_permit_empty_plte __mask_png_permit_empty_plte
#define png_set_flush __mask_png_set_flush
#define png_write_flush __mask_png_write_flush
#define png_start_read_image __mask_png_start_read_image
#define png_read_update_info __mask_png_read_update_info
#define png_read_rows __mask_png_read_rows
#define png_read_row __mask_png_read_row
#define png_read_image __mask_png_read_image
#define png_write_row __mask_png_write_row
#define png_write_rows __mask_png_write_rows
#define png_write_image __mask_png_write_image
#define png_write_end __mask_png_write_end
#define png_read_end __mask_png_read_end
#define png_destroy_info_struct __mask_png_destroy_info_struct
#define png_destroy_read_struct __mask_png_destroy_read_struct
#define png_read_destroy __mask_png_read_destroy
#define png_destroy_write_struct __mask_png_destroy_write_struct
#define png_write_destroy __mask_png_write_destroy
#define png_set_crc_action __mask_png_set_crc_action
#define png_set_filter __mask_png_set_filter
#define png_set_filter_heuristics __mask_png_set_filter_heuristics
#define png_set_compression_level __mask_png_set_compression_level
#define png_set_compression_mem_level __mask_png_set_compression_mem_level
#define png_set_compression_strategy __mask_png_set_compression_strategy
#define png_set_compression_window_bits __mask_png_set_compression_window_bits
#define png_set_compression_method __mask_png_set_compression_method
#define png_init_io __mask_png_init_io
#define png_set_error_fn __mask_png_set_error_fn
#define png_get_error_ptr __mask_png_get_error_ptr
#define png_set_write_fn __mask_png_set_write_fn
#define png_set_read_fn __mask_png_set_read_fn
#define png_get_io_ptr __mask_png_get_io_ptr
#define png_set_read_status_fn __mask_png_set_read_status_fn
#define png_set_write_status_fn __mask_png_set_write_status_fn
#define png_set_mem_fn __mask_png_set_mem_fn
#define png_get_mem_ptr __mask_png_get_mem_ptr
#define png_set_read_user_transform_fn __mask_png_set_read_user_transform_fn
#define png_set_write_user_transform_fn __mask_png_set_write_user_transform_fn
#define png_set_user_transform_info __mask_png_set_user_transform_info
#define png_get_user_transform_ptr __mask_png_get_user_transform_ptr
#define png_set_read_user_chunk_fn __mask_png_set_read_user_chunk_fn
#define png_get_user_chunk_ptr __mask_png_get_user_chunk_ptr
#define png_set_progressive_read_fn __mask_png_set_progressive_read_fn
#define png_get_progressive_ptr __mask_png_get_progressive_ptr
#define png_process_data __mask_png_process_data
#define png_progressive_combine_row __mask_png_progressive_combine_row
#define png_malloc __mask_png_malloc
#define png_free __mask_png_free
#define png_free_data __mask_png_free_data
#define png_data_freer __mask_png_data_freer
#define png_malloc_default __mask_png_malloc_default
#define png_free_default __mask_png_free_default
#define png_far_to_near __mask_png_far_to_near
#define png_error __mask_png_error
#define png_chunk_error __mask_png_chunk_error
#define png_warning __mask_png_warning
#define png_chunk_warning __mask_png_chunk_warning
#define png_get_valid __mask_png_get_valid
#define png_get_rowbytes __mask_png_get_rowbytes
#define png_get_rows __mask_png_get_rows
#define png_set_rows __mask_png_set_rows
#define png_get_channels __mask_png_get_channels
#define png_get_image_width __mask_png_get_image_width
#define png_get_image_height __mask_png_get_image_height
#define png_get_bit_depth __mask_png_get_bit_depth
#define png_get_color_type __mask_png_get_color_type
#define png_get_filter_type __mask_png_get_filter_type
#define png_get_interlace_type __mask_png_get_interlace_type
#define png_get_compression_type __mask_png_get_compression_type
#define png_get_pixels_per_meter __mask_png_get_pixels_per_meter
#define png_get_x_pixels_per_meter __mask_png_get_x_pixels_per_meter
#define png_get_y_pixels_per_meter __mask_png_get_y_pixels_per_meter
#define png_get_pixel_aspect_ratio __mask_png_get_pixel_aspect_ratio
#define png_get_x_offset_pixels __mask_png_get_x_offset_pixels
#define png_get_y_offset_pixels __mask_png_get_y_offset_pixels
#define png_get_x_offset_microns __mask_png_get_x_offset_microns
#define png_get_y_offset_microns __mask_png_get_y_offset_microns
#define png_get_signature __mask_png_get_signature
#define png_get_bKGD __mask_png_get_bKGD
#define png_set_bKGD __mask_png_set_bKGD
#define png_get_cHRM __mask_png_get_cHRM
#define png_get_cHRM_fixed __mask_png_get_cHRM_fixed
#define png_set_cHRM __mask_png_set_cHRM
#define png_set_cHRM_fixed __mask_png_set_cHRM_fixed
#define png_get_gAMA __mask_png_get_gAMA
#define png_get_gAMA_fixed __mask_png_get_gAMA_fixed
#define png_set_gAMA __mask_png_set_gAMA
#define png_set_gAMA_fixed __mask_png_set_gAMA_fixed
#define png_get_hIST __mask_png_get_hIST
#define png_set_hIST __mask_png_set_hIST
#define png_get_IHDR __mask_png_get_IHDR
#define png_set_IHDR __mask_png_set_IHDR
#define png_get_oFFs __mask_png_get_oFFs
#define png_set_oFFs __mask_png_set_oFFs
#define png_get_pCAL __mask_png_get_pCAL
#define png_set_pCAL __mask_png_set_pCAL
#define png_get_pHYs __mask_png_get_pHYs
#define png_set_pHYs __mask_png_set_pHYs
#define png_get_PLTE __mask_png_get_PLTE
#define png_set_PLTE __mask_png_set_PLTE
#define png_get_sBIT __mask_png_get_sBIT
#define png_set_sBIT __mask_png_set_sBIT
#define png_get_sRGB __mask_png_get_sRGB
#define png_set_sRGB __mask_png_set_sRGB
#define png_set_sRGB_gAMA_and_cHRM __mask_png_set_sRGB_gAMA_and_cHRM
#define png_get_iCCP __mask_png_get_iCCP
#define png_set_iCCP __mask_png_set_iCCP
#define png_get_sPLT __mask_png_get_sPLT
#define png_set_sPLT __mask_png_set_sPLT
#define png_get_text __mask_png_get_text
#define png_set_text __mask_png_set_text
#define png_get_tIME __mask_png_get_tIME
#define png_set_tIME __mask_png_set_tIME
#define png_get_tRNS __mask_png_get_tRNS
#define png_set_tRNS __mask_png_set_tRNS
#define png_get_sCAL __mask_png_get_sCAL
#define png_get_sCAL_s __mask_png_get_sCAL_s
#define png_set_sCAL __mask_png_set_sCAL
#define png_set_sCAL_s __mask_png_set_sCAL_s
#define png_set_keep_unknown_chunks __mask_png_set_keep_unknown_chunks
#define png_set_unknown_chunks __mask_png_set_unknown_chunks
#define png_set_unknown_chunk_location __mask_png_set_unknown_chunk_location
#define png_get_unknown_chunks __mask_png_get_unknown_chunks
#define png_set_invalid __mask_png_set_invalid
#define png_read_png __mask_png_read_png
#define png_write_png __mask_png_write_png
#define png_sig_bytes __mask_png_sig_bytes
#define png_get_copyright __mask_png_get_copyright
#define png_get_header_ver __mask_png_get_header_ver
#define png_get_header_version __mask_png_get_header_version
#define png_get_libpng_ver __mask_png_get_libpng_ver
#define png_read_init __mask_png_read_init
#define png_read_init_2 __mask_png_read_init_2
#define png_create_struct __mask_png_create_struct
#define png_destroy_struct __mask_png_destroy_struct
#define png_create_struct_2 __mask_png_create_struct_2
#define png_destroy_struct_2 __mask_png_destroy_struct_2
#define png_info_destroy __mask_png_info_destroy
#define png_zalloc __mask_png_zalloc
#define png_zfree __mask_png_zfree
#define png_reset_crc __mask_png_reset_crc
#define png_write_data __mask_png_write_data
#define png_read_data __mask_png_read_data
#define png_crc_read __mask_png_crc_read
#define png_decompress_chunk __mask_png_decompress_chunk
#define png_crc_finish __mask_png_crc_finish
#define png_crc_error __mask_png_crc_error
#define png_calculate_crc __mask_png_calculate_crc
#define png_flush __mask_png_flush
#define png_save_uint_32 __mask_png_save_uint_32
#define png_save_int_32 __mask_png_save_int_32
#define png_save_uint_16 __mask_png_save_uint_16
#define png_write_sig __mask_png_write_sig
#define png_write_IHDR __mask_png_write_IHDR
#define png_write_PLTE __mask_png_write_PLTE
#define png_write_IDAT __mask_png_write_IDAT
#define png_write_IEND __mask_png_write_IEND
#define png_write_gAMA __mask_png_write_gAMA
#define png_write_gAMA_fixed __mask_png_write_gAMA_fixed
#define png_write_sBIT __mask_png_write_sBIT
#define png_write_cHRM __mask_png_write_cHRM
#define png_write_cHRM_fixed __mask_png_write_cHRM_fixed
#define png_write_sRGB __mask_png_write_sRGB
#define png_write_iCCP __mask_png_write_iCCP
#define png_write_sPLT __mask_png_write_sPLT
#define png_write_tRNS __mask_png_write_tRNS
#define png_write_bKGD __mask_png_write_bKGD
#define png_write_hIST __mask_png_write_hIST
#define png_check_keyword __mask_png_check_keyword
#define png_write_tEXt __mask_png_write_tEXt
#define png_write_zTXt __mask_png_write_zTXt
#define png_write_iTXt __mask_png_write_iTXt
#define png_write_oFFs __mask_png_write_oFFs
#define png_write_pCAL __mask_png_write_pCAL
#define png_write_pHYs __mask_png_write_pHYs
#define png_write_tIME __mask_png_write_tIME
#define png_write_sCAL __mask_png_write_sCAL
#define png_write_sCAL_s __mask_png_write_sCAL_s
#define png_write_finish_row __mask_png_write_finish_row
#define png_write_start_row __mask_png_write_start_row
#define png_build_gamma_table __mask_png_build_gamma_table
#define png_combine_row __mask_png_combine_row
#define png_do_read_interlace __mask_png_do_read_interlace
#define png_do_write_interlace __mask_png_do_write_interlace
#define png_read_filter_row __mask_png_read_filter_row
#define png_write_find_filter __mask_png_write_find_filter
#define png_write_filtered_row __mask_png_write_filtered_row
#define png_read_finish_row __mask_png_read_finish_row
#define png_read_start_row __mask_png_read_start_row
#define png_read_transform_info __mask_png_read_transform_info
#define png_do_read_filler __mask_png_do_read_filler
#define png_do_read_swap_alpha __mask_png_do_read_swap_alpha
#define png_do_write_swap_alpha __mask_png_do_write_swap_alpha
#define png_do_read_invert_alpha __mask_png_do_read_invert_alpha
#define png_do_write_invert_alpha __mask_png_do_write_invert_alpha
#define png_do_strip_filler __mask_png_do_strip_filler
#define png_do_swap __mask_png_do_swap
#define png_do_packswap __mask_png_do_packswap
#define png_do_rgb_to_gray __mask_png_do_rgb_to_gray
#define png_do_gray_to_rgb __mask_png_do_gray_to_rgb
#define png_do_unpack __mask_png_do_unpack
#define png_do_unshift __mask_png_do_unshift
#define png_do_invert __mask_png_do_invert
#define png_do_chop __mask_png_do_chop
#define png_do_dither __mask_png_do_dither
#define png_correct_palette __mask_png_correct_palette
#define png_do_bgr __mask_png_do_bgr
#define png_do_pack __mask_png_do_pack
#define png_do_shift __mask_png_do_shift
#define png_do_background __mask_png_do_background
#define png_do_gamma __mask_png_do_gamma
#define png_do_expand_palette __mask_png_do_expand_palette
#define png_do_expand __mask_png_do_expand
#define png_handle_IHDR __mask_png_handle_IHDR
#define png_handle_PLTE __mask_png_handle_PLTE
#define png_handle_IEND __mask_png_handle_IEND
#define png_handle_bKGD __mask_png_handle_bKGD
#define png_handle_cHRM __mask_png_handle_cHRM
#define png_handle_gAMA __mask_png_handle_gAMA
#define png_handle_hIST __mask_png_handle_hIST
#define png_handle_iCCP __mask_png_handle_iCCP
#define png_handle_iTXt __mask_png_handle_iTXt
#define png_handle_oFFs __mask_png_handle_oFFs
#define png_handle_pCAL __mask_png_handle_pCAL
#define png_handle_pHYs __mask_png_handle_pHYs
#define png_handle_sBIT __mask_png_handle_sBIT
#define png_handle_sCAL __mask_png_handle_sCAL
#define png_handle_sPLT __mask_png_handle_sPLT
#define png_handle_sRGB __mask_png_handle_sRGB
#define png_handle_tEXt __mask_png_handle_tEXt
#define png_handle_tIME __mask_png_handle_tIME
#define png_handle_tRNS __mask_png_handle_tRNS
#define png_handle_zTXt __mask_png_handle_zTXt
#define png_handle_as_unknown __mask_png_handle_as_unknown
#define png_handle_unknown __mask_png_handle_unknown
#define png_check_chunk_name __mask_png_check_chunk_name
#define png_do_read_transformations __mask_png_do_read_transformations
#define png_do_write_transformations __mask_png_do_write_transformations
#define png_init_read_transformations __mask_png_init_read_transformations
#define png_push_read_chunk __mask_png_push_read_chunk
#define png_push_read_sig __mask_png_push_read_sig
#define png_push_check_crc __mask_png_push_check_crc
#define png_push_crc_skip __mask_png_push_crc_skip
#define png_push_crc_finish __mask_png_push_crc_finish
#define png_push_fill_buffer __mask_png_push_fill_buffer
#define png_push_save_buffer __mask_png_push_save_buffer
#define png_push_restore_buffer __mask_png_push_restore_buffer
#define png_push_read_IDAT __mask_png_push_read_IDAT
#define png_process_IDAT_data __mask_png_process_IDAT_data
#define png_push_process_row __mask_png_push_process_row
#define png_push_handle_unknown __mask_png_push_handle_unknown
#define png_push_have_info __mask_png_push_have_info
#define png_push_have_end __mask_png_push_have_end
#define png_push_have_row __mask_png_push_have_row
#define png_push_read_end __mask_png_push_read_end
#define png_process_some_data __mask_png_process_some_data
#define png_read_push_finish_row __mask_png_read_push_finish_row
#define png_push_handle_tEXt __mask_png_push_handle_tEXt
#define png_push_read_tEXt __mask_png_push_read_tEXt
#define png_push_handle_zTXt __mask_png_push_handle_zTXt
#define png_push_read_zTXt __mask_png_push_read_zTXt
#define png_push_handle_iTXt __mask_png_push_handle_iTXt
#define png_push_read_iTXt __mask_png_push_read_iTXt
#endif /* WIN32 */
