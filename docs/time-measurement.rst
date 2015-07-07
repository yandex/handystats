.. _time-measurement:

Time Measurement
================

**Fast** and **accurate** elapsed time measurement is an essential part of any performance monitoring system,
espesially system that is aimed to run in a production environment.

Unfortunately, there's no the one true solution for time measurement problem.
Various types of operation system and processor architecture provide different methods with its pluses and minuses.

Clock Sources
-------------

Time measurement is based on an ability to get current time stamp in arbitrary unit (for example, millisecons or the number of cycles).

On POSIX systems three the most widely used sources for such "time stamps" are Time Stamp Counter, High Precision Event Timer and ACPI Power Management Timer.

Time Stamp Counter
++++++++++++++++++

One of the most fast and accurate approach to measure elapsed time is by using `Time Stamp Counter <http://en.wikipedia.org/wiki/Time_Stamp_Counter>`_.

The Time Stamp Counter (TSC) is a special 64-bit register which counts the number of processor's cycles since reset.
The TSC provides the highest-resolution timing information available for that processor.

But despite its high accurancy and low overhead the TSC is hard to use now:

    The time stamp counter has, until recently, been an excellent high-resolution, low-overhead way of getting CPU timing information.
    With the advent of multi-core/hyper-threaded CPUs, systems with multiple CPUs, and hibernating operating systems, the TSC cannot be relied on
    to provide accurate results — unless great care is taken to correct the possible flaws: rate of tick and whether all cores (processors) have
    identical values in their time-keeping registers. 
    There is no promise that the timestamp counters of multiple CPUs on a single motherboard will be synchronized.
    In such cases, programmers can only get reliable results by locking their code to a single CPU.
    Even then, the CPU speed may change due to power-saving measures taken by the OS or BIOS, or the system may be hibernated
    and later resumed (resetting the time stamp counter).
    In those latter cases, to stay relevant, the counter must be recalibrated periodically (according to the time resolution the application requires).

Contemporary Intel and AMD processors provide a serializing instruction :code:`RDTSCP` to read the TSC and an indetifier indicating on which CPU the TSC was read
unlike the regular instruction :code:`RDTSC` which just reads the TSC.
CPU's identifier is needed becase time stamp counters from different CPUs are not guaranteed to be synchronized.

Also an enhanced versions of TSC (:code:`constant_tsc` and more general :code:`invariant_tsc` and :code:`nonstop_tsc`) are provided by modern Intel processors
that run at the processor's maximum rate regardless of the actual CPU running rate.

Thus, reliance on the TSC reduces portability, as other processors may not have a similar feature and its properties.
Furthermore, to convert the number of cycles to more convinient units such as millisecons TSC's rate has to be determinied accurately.

Read the following documents for more details on the Time Stamp Counter:

- `TSC Synchronization Across Cores <https://software.intel.com/en-us/forums/topic/388964>`_
- `Game Timing and Multicore Processors <http://msdn.microsoft.com/en-us/library/windows/desktop/ee417693%28v=vs.85%29.aspx>`_
- `Timestamping <https://access.redhat.com/site/documentation/en-US/Red_Hat_Enterprise_MRG/2/html/Realtime_Reference_Guide/chap-Realtime_Reference_Guide-Timestamping.html>`_ from the Red Hat MRG's documentation
- `Wikipedia's page <http://en.wikipedia.org/wiki/Time_Stamp_Counter>`_

High Precision Event Timer
++++++++++++++++++++++++++

`HPET <http://en.wikipedia.org/wiki/High_Precision_Event_Timer>`_ is a hardware timer implemented as a 64-bit up-counter counting at a frequency of at least 10 MHz.

To access the HPET's time stamp one can read it from special memory locations.

Comparing to the TSC, reading from which is, basically, reading a register from the processor, reading from the HPET clock is significantly slower
for measuring time for high rate events.
Thus, the Time Stamp Counter is preferred over the HPET as a clock source.

For more information about HPET see the following documents:

- `Timestamping <https://access.redhat.com/site/documentation/en-US/Red_Hat_Enterprise_MRG/2/html/Realtime_Reference_Guide/chap-Realtime_Reference_Guide-Timestamping.html>`_ from the Red Hat MRG's documentation
- `Wikipedia's page <http://en.wikipedia.org/wiki/High_Precision_Event_Timer>`_

ACPI Power Management Timer
+++++++++++++++++++++++++++

APCI PM Timer is the slowest timer among specified above and is used in case of the absence of the TSC and the HPET.

See the following documents for more details:

- `Timestamping <https://access.redhat.com/site/documentation/en-US/Red_Hat_Enterprise_MRG/2/html/Realtime_Reference_Guide/chap-Realtime_Reference_Guide-Timestamping.html>`_ from the Red Hat MRG's documentation


POSIX Clocks
------------

*POSIX clocks* is a standard for implementing and representing time sources.

Nevertheless, POSIX doesn't require any particular underlying hardware clock source for implementing clocks.
Thus, POSIX clocks can be seen as an abstraction over hardware clock sources that defines types of clock with its properties.

Linux supports the following types of clock that are of interest to us:

- **CLOCK_REALTIME**

  The system-wide real time (wall time) clock.
  This clock measures the amount of time that has elapsed since 00:00:00 January 1, 1970 Greenwich Mean Time (GMT).
  It can be modified by an user with the right privileges.
  Thus, the clock can jump forwards and backwards as the system time-of-day clock is changed, including by NTP.

- **CLOCK_MONOTONIC**

  Clock that represents monotonic time since some unspecified starting point, such as system boot.
  This clock is not settable by any process and is not affected by discontinous jumps in the system time,
  but is affected by the incremental adjustments performed by :code:`adjtime()` and NTP.

- **CLOCK_MONOTONIC_RAW** (since Linux 2.6.32, Linux-specific)

  Similar to CLOCK_MONOTONIC, but provides access to a raw hardware-based time that is not subject to
  the incremental adjustments performed by :code:`adjtime()` or NTP.

POSIX defines CLOCK_REALTIME and CLOCK_MONOTONIC types of clock, but it requires only CLOCK_REALTIME.
Therefore, while Linux provides all of these clocks, on other systems analogous to CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW clocks should be used
or, in case of absence of those, CLOCK_REALTIME.

Considering issues and complexity of using hardware clock sources specified above as well as for code portability using POSIX clocks is preferred for most systems.

POSIX provides the :code:`clock_gettime()` interface for obtaining the time of a specific clock.
More useful is that the function allows for nanosecond precision returning result in :code:`timespec` structure.

For more information on POSIX clocks see the following:

- `Timestamping <https://access.redhat.com/site/documentation/en-US/Red_Hat_Enterprise_MRG/2/html/Realtime_Reference_Guide/chap-Realtime_Reference_Guide-Timestamping.html>`_ from the Red Hat MRG's documentation
- *Linux System Programming* by Robert Love
- `clock_gettime() man page <http://linux.die.net/man/3/clock_gettime>`_

Time Intervals And Timestamps
-----------------------------

Possible jumps of CLOCK_REALTIME make it unreliable for measuring time intervals, thus, one or the other clocks should be used instead, if available.
The same way, the time from CLOCK_MONOTONIC and analogous cannot be mapped to the current real-world date and time.

This shows two conceptual different time-based problems that should be solved by performance monitoring systems:

- **measuring time intervals**

  For solving this problem any high-precision monotonic clock could be used, even hardware clock source.
  The only restriction is an ability to accurately convert time interval's duration from clock's units to more convinient units, such as nanoseconds.

- **timestamping events**

  This problem raises only when user wants to work with event timestamps.
  Inside the system any suitable clock can be used, but for the "*outside world*" timestamp should be in consensual form,
  such as `Unix time <http://en.wikipedia.org/wiki/Unix_time>`_.

Clock Concept
-------------

To hide implementation details and underlying clock type from users and other parts of the handystats library **Clock concept**
similar to `C++ Clock concept <http://en.cppreference.com/w/cpp/concept/Clock>`_ has been introduced:

- Type names:

  - :code:`time_point` -- represents time point from clock's epoch

  - :code:`duration` -- represents duration between time points

- Member functions:

  - :code:`static time_point now() noexcept` -- returns current time point

- Non-member functions:

  - :code:`std::chrono::system_clock::time_point to_system_time(const clock::time_point&)`
    -- converts :code:`clock::time_point` to system-wide clock's time represented by :code:`std::chrono::system_clock::time_point`

    This function should perform conversion from :code:`clock::time_point` which may represent hardware clock source value, such as TSC,
    and have no connection to system-wide clock.
    Still, as the handystats library uses clock concept's :code:`time_point` for both measuring time intervals and timestamping events
    conversion is needed for users to work with events timestamps at runtime.

  - :code:`duration_cast` from and to :code:`clock::duration`
    -- performs conversion between :code:`clock::duration` and :code:`std::chrono::duration`

    This function should perform conversion between :code:`clock::duration` and :code:`std::chrono::duration`.
    The former may represent intervals in terms of hardware clock's ticks and have no explicit connection to convenient time units,
    while the latter represents time intervals in terms of convenient time units, such as :code:`std::chrono::milliseconds`.

Note that :code:`time_point` and :code:`duration` types can have no connection to :code:`std::chrono`,
but this types try to follow corresponding :code:`std::chrono` public interfaces.

The handystats library provides the following :code:`typedef` that represents library-wide clock:

.. code-block:: cpp

    namespace handystats { namespace chrono {

    typedef <clock concept implementation> clock;

    }}

Implementation Details
----------------------

The handystats library implements library-wide clock using the Time Stamp Counter register as the most precise and fast hardware clock source.
To read the value from the TSC :code:`RDTSCP` serializing instruction is used.

Considering specified above caveats on using the TSC we're aimed on processor architectures and operation systems that support **constant TSC**
and **RDTSCP** serializing instruction.

The Time Stamp Counter Rate
+++++++++++++++++++++++++++

For measuring time intervals calibration between the number of cycles and time units, specifically nanoseconds, should be performed.
Thus, **the TSC's rate** should be determined.

The TSC's rate is determined by multiple interval measurements by TSC and CLOCK_MONOTONIC simultaneously
and choosing median frequency between measurements.
To find corresponding pair of TSC and CLOCK_MONOTONIC values at start and end of interval measurement
CLOCK_MONOTONIC time retrieval is surrounded by RDTSCP calls.
And pair of CLOCK_MONOTONIC time and average of TSC values is formed only if the difference between TSC values is acceptable.
Otherwise, determination of corresponding pair of TSC and CLOCK_MONOTONIC values is repeated.

The TSC's rate determination should be performed at startup.
To this purpose we mark the TSC initializing function with :code:`__attribute__((constructor))` to be invoked at load-time.

Note, that :code:`__attribute__((constructor))` is GCC-specific semantics, thereby we limit the set of supported compilers to GCC and Clang.
See :ref:`requirements` for more details.

Cycles Count To System Time Conversion
++++++++++++++++++++++++++++++++++++++

To convert the number of cycles that counts from some unspecified point in time to absolute system time
correlation between system time (represented by C++11's :code:`std::chrono::system_clock` or POSIX's :code:`CLOCK_REALTIME`)
and the number of cycles should be known.

At this point there should be a "*tied*"pair of timestamp in terms of the of cycles and timestamp of system clock that correspond to the same point in time.
Moreover, considering possible system time updates and adjustments the tied pair of internal and system time should be updated periodically.

Such conversion is performed by :code:`handystats::chrono::to_system_time` function described above.
Considering our focus on multithreaded applications there's no limit on the number of concurrent calls to the function,
thus updates of the tied pair of internal and system time and calls to the :code:`handystats::chrono::to_system_time` should be **thread-safe**.

To ease implementation of the update of the tied pair, let's do some math:

- let :math:`R` be the rate of the TSC,
- let :math:`T_{tsc}, T_{sys}` be the tied pair of internal :math:`T_{tsc}` and system :math:`T_{sys}` time,
- let :math:`t_{tsc}` be current internal time and we want to find current system :math:`t_{sys}`.

The following formula is the solution:

.. math::

    t_{sys} = T_{sys} + \frac{t_{tsc} - T_{tsc}}{R}

This formula can be transformed to

.. math::

    t_{sys} = \frac{t_{tsc}}{R} + (T_{sys} - \frac{T_{tsc}}{R})

The last term in brackets is an **offset** that fully replaces the tied pair.
Thus, the only we need to update is single value instead of a pair.
And such update of the offset can be performed in a **lock-free** manner.
