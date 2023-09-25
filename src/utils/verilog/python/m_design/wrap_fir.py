def design_fir_filter(fs, pass_type, f_range, n_cycles=3, n_seconds=None):
    """Design an FIR filter.

    Parameters
    ----------
    fs : float
        Sampling rate, in Hz.
    pass_type : {'bandpass', 'bandstop', 'lowpass', 'highpass'}
        Which kind of filter to apply:

        * 'bandpass': apply a bandpass filter
        * 'bandstop': apply a bandstop (notch) filter
        * 'lowpass': apply a lowpass filter
        * 'highpass' : apply a highpass filter
    f_range : tuple of (float, float) or float
        Cutoff frequency(ies) used for filter, specified as f_lo & f_hi.
        For 'bandpass' & 'bandstop', must be a tuple.
        For 'lowpass' or 'highpass', can be a float that specifies pass frequency, or can be
        a tuple and is assumed to be (None, f_hi) for 'lowpass', and (f_lo, None) for 'highpass'.
    n_cycles : float, optional, default: 3
        Length of filter, in number of cycles, defined at the 'f_lo' frequency.
        This parameter is overwritten by `n_seconds`, if provided.
    n_seconds : float or None, optional
        Length of filter, in seconds. This parameter overwrites `n_cycles`.

    Returns
    -------
    filter_coefs : 1d array
        The filter coefficients for an FIR filter.

    Examples
    --------
    Create the filter coefficients for an FIR filter:

    >>> filter_coefs = design_fir_filter(fs=500, pass_type='bandpass', f_range=(1, 25))
    """

    # Check filter definition
    f_lo, f_hi = check_filter_definition(pass_type, f_range)
    filt_len = compute_filter_length(fs, pass_type, f_lo, f_hi, n_cycles, n_seconds)

    if pass_type == 'bandpass':
        filter_coefs = firwin(filt_len, (f_lo, f_hi), pass_zero=False, fs=fs)
    elif pass_type == 'bandstop':
        filter_coefs = firwin(filt_len, (f_lo, f_hi), fs=fs)
    elif pass_type == 'highpass':
        filter_coefs = firwin(filt_len, f_lo, pass_zero=False, fs=fs)
    elif pass_type == 'lowpass':
        filter_coefs = firwin(filt_len, f_hi, fs=fs)

    return filter_coefs 