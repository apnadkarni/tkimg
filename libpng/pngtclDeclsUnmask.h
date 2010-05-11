#ifdef WIN32
/* Restore the usage of the original names */
#undef png_access_version_number
#undef png_set_sig_bytes
#undef png_sig_cmp
#undef png_create_read_struct
#undef png_create_write_struct
#undef png_get_compression_buffer_size
#undef png_set_compression_buffer_size
#undef png_reset_zstream
#undef png_create_read_struct_2
#undef png_create_write_struct_2
#undef png_write_chunk
#undef png_write_chunk_start
#undef png_write_chunk_data
#undef png_write_chunk_end
#undef png_create_info_struct
#undef png_info_init_3
#undef png_write_info_before_PLTE
#undef png_write_info
#undef png_read_info
#undef png_convert_to_rfc1123
#undef png_convert_from_struct_tm
#undef png_convert_from_time_t
#undef png_set_expand
#undef png_set_palette_to_rgb
#undef png_set_tRNS_to_alpha
#undef png_set_bgr
#undef png_set_gray_to_rgb
#undef png_set_rgb_to_gray
#undef png_set_rgb_to_gray_fixed
#undef png_get_rgb_to_gray_status
#undef png_build_grayscale_palette
#undef png_set_strip_alpha
#undef png_set_swap_alpha
#undef png_set_invert_alpha
#undef png_set_filler
#undef png_set_swap
#undef png_set_packing
#undef png_set_packswap
#undef png_set_shift
#undef png_set_interlace_handling
#undef png_set_invert_mono
#undef png_set_background
#undef png_set_strip_16
#undef png_set_dither
#undef png_set_gamma
#undef png_permit_empty_plte
#undef png_set_flush
#undef png_write_flush
#undef png_start_read_image
#undef png_read_update_info
#undef png_read_rows
#undef png_read_row
#undef png_read_image
#undef png_write_row
#undef png_write_rows
#undef png_write_image
#undef png_write_end
#undef png_read_end
#undef png_destroy_info_struct
#undef png_destroy_read_struct
#undef png_destroy_write_struct
#undef png_set_crc_action
#undef png_set_filter
#undef png_set_filter_heuristics
#undef png_set_compression_level
#undef png_set_compression_mem_level
#undef png_set_compression_strategy
#undef png_set_compression_window_bits
#undef png_set_compression_method
#undef png_init_io
#undef png_set_error_fn
#undef png_get_error_ptr
#undef png_set_write_fn
#undef png_set_read_fn
#undef png_get_io_ptr
#undef png_set_read_status_fn
#undef png_set_write_status_fn
#undef png_set_mem_fn
#undef png_get_mem_ptr
#undef png_set_read_user_transform_fn
#undef png_set_write_user_transform_fn
#undef png_set_user_transform_info
#undef png_get_user_transform_ptr
#undef png_set_read_user_chunk_fn
#undef png_get_user_chunk_ptr
#undef png_set_progressive_read_fn
#undef png_get_progressive_ptr
#undef png_process_data
#undef png_progressive_combine_row
#undef png_malloc
#undef png_free
#undef png_free_data
#undef png_data_freer
#undef png_malloc_default
#undef png_free_default
#undef png_far_to_near
#undef png_error
#undef png_chunk_error
#undef png_warning
#undef png_chunk_warning
#undef png_get_valid
#undef png_get_rowbytes
#undef png_get_rows
#undef png_set_rows
#undef png_get_channels
#undef png_get_image_width
#undef png_get_image_height
#undef png_get_bit_depth
#undef png_get_color_type
#undef png_get_filter_type
#undef png_get_interlace_type
#undef png_get_compression_type
#undef png_get_pixels_per_meter
#undef png_get_x_pixels_per_meter
#undef png_get_y_pixels_per_meter
#undef png_get_pixel_aspect_ratio
#undef png_get_x_offset_pixels
#undef png_get_y_offset_pixels
#undef png_get_x_offset_microns
#undef png_get_y_offset_microns
#undef png_get_signature
#undef png_get_bKGD
#undef png_set_bKGD
#undef png_get_cHRM
#undef png_get_cHRM_fixed
#undef png_set_cHRM
#undef png_set_cHRM_fixed
#undef png_get_gAMA
#undef png_get_gAMA_fixed
#undef png_set_gAMA
#undef png_set_gAMA_fixed
#undef png_get_hIST
#undef png_set_hIST
#undef png_get_IHDR
#undef png_set_IHDR
#undef png_get_oFFs
#undef png_set_oFFs
#undef png_get_pCAL
#undef png_set_pCAL
#undef png_get_pHYs
#undef png_set_pHYs
#undef png_get_PLTE
#undef png_set_PLTE
#undef png_get_sBIT
#undef png_set_sBIT
#undef png_get_sRGB
#undef png_set_sRGB
#undef png_set_sRGB_gAMA_and_cHRM
#undef png_get_iCCP
#undef png_set_iCCP
#undef png_get_sPLT
#undef png_set_sPLT
#undef png_get_text
#undef png_set_text
#undef png_get_tIME
#undef png_set_tIME
#undef png_get_tRNS
#undef png_set_tRNS
#undef png_get_sCAL
#undef png_get_sCAL_s
#undef png_set_sCAL
#undef png_set_sCAL_s
#undef png_set_keep_unknown_chunks
#undef png_set_unknown_chunks
#undef png_set_unknown_chunk_location
#undef png_get_unknown_chunks
#undef png_set_invalid
#undef png_read_png
#undef png_write_png
#undef png_sig_bytes
#undef png_get_copyright
#undef png_get_header_ver
#undef png_get_header_version
#undef png_get_libpng_ver
#undef png_read_init
#undef png_read_init_2
#undef png_create_struct
#undef png_destroy_struct
#undef png_create_struct_2
#undef png_destroy_struct_2
#undef png_info_destroy
#undef png_zalloc
#undef png_zfree
#undef png_reset_crc
#undef png_write_data
#undef png_read_data
#undef png_crc_read
#undef png_decompress_chunk
#undef png_crc_finish
#undef png_crc_error
#undef png_calculate_crc
#undef png_flush
#undef png_save_uint_32
#undef png_save_int_32
#undef png_save_uint_16
#undef png_write_sig
#undef png_write_IHDR
#undef png_write_PLTE
#undef png_write_IDAT
#undef png_write_IEND
#undef png_write_gAMA
#undef png_write_gAMA_fixed
#undef png_write_sBIT
#undef png_write_cHRM
#undef png_write_cHRM_fixed
#undef png_write_sRGB
#undef png_write_iCCP
#undef png_write_sPLT
#undef png_write_tRNS
#undef png_write_bKGD
#undef png_write_hIST
#undef png_check_keyword
#undef png_write_tEXt
#undef png_write_zTXt
#undef png_write_iTXt
#undef png_write_oFFs
#undef png_write_pCAL
#undef png_write_pHYs
#undef png_write_tIME
#undef png_write_sCAL
#undef png_write_sCAL_s
#undef png_write_finish_row
#undef png_write_start_row
#undef png_build_gamma_table
#undef png_combine_row
#undef png_do_read_interlace
#undef png_do_write_interlace
#undef png_read_filter_row
#undef png_write_find_filter
#undef png_write_filtered_row
#undef png_read_finish_row
#undef png_read_start_row
#undef png_read_transform_info
#undef png_do_read_filler
#undef png_do_read_swap_alpha
#undef png_do_write_swap_alpha
#undef png_do_read_invert_alpha
#undef png_do_write_invert_alpha
#undef png_do_strip_filler
#undef png_do_swap
#undef png_do_packswap
#undef png_do_rgb_to_gray
#undef png_do_gray_to_rgb
#undef png_do_unpack
#undef png_do_unshift
#undef png_do_invert
#undef png_do_chop
#undef png_do_dither
#undef png_correct_palette
#undef png_do_bgr
#undef png_do_pack
#undef png_do_shift
#undef png_do_background
#undef png_do_gamma
#undef png_do_expand_palette
#undef png_do_expand
#undef png_handle_IHDR
#undef png_handle_PLTE
#undef png_handle_IEND
#undef png_handle_bKGD
#undef png_handle_cHRM
#undef png_handle_gAMA
#undef png_handle_hIST
#undef png_handle_iCCP
#undef png_handle_iTXt
#undef png_handle_oFFs
#undef png_handle_pCAL
#undef png_handle_pHYs
#undef png_handle_sBIT
#undef png_handle_sCAL
#undef png_handle_sPLT
#undef png_handle_sRGB
#undef png_handle_tEXt
#undef png_handle_tIME
#undef png_handle_tRNS
#undef png_handle_zTXt
#undef png_handle_as_unknown
#undef png_handle_unknown
#undef png_check_chunk_name
#undef png_do_read_transformations
#undef png_do_write_transformations
#undef png_init_read_transformations
#undef png_push_read_chunk
#undef png_push_read_sig
#undef png_push_check_crc
#undef png_push_crc_skip
#undef png_push_crc_finish
#undef png_push_fill_buffer
#undef png_push_save_buffer
#undef png_push_restore_buffer
#undef png_push_read_IDAT
#undef png_process_IDAT_data
#undef png_push_process_row
#undef png_push_handle_unknown
#undef png_push_have_info
#undef png_push_have_end
#undef png_push_have_row
#undef png_push_read_end
#undef png_process_some_data
#undef png_read_push_finish_row
#undef png_push_handle_tEXt
#undef png_push_read_tEXt
#undef png_push_handle_zTXt
#undef png_push_read_zTXt
#undef png_push_handle_iTXt
#undef png_push_read_iTXt
#endif /* WIN32 */

