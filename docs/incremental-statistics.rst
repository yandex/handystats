.. _incremental-statistics:

Incremental Statistics
======================

*Incremental statistics* is the base of handystats metrics' data aggregation mechanism.
Thus understanding this type of statistics, its pros and cons is crucial.

Definition
----------

**Incremental statistics** are such statistics over dataset of pairs ``(value, timestamp)`` that can be maintained and updated on each new data with constant time and space complexity.

Examples of incremental statistics are:

* min
* max
* sum
* moving average

One of the main properties of incremental statistics is that no raw input data have to be stored.
This could be disadvantage if you would like to retrieve some special statistics from input data that are not presented in handystats library.
But this approach allows us to keep memory consumption low.

Interval Statistics
-------------------

Some simple incremental statistics such as *min*, *max*, *sum*, *count* and *mean* act on complete dataset.
While knowing total count or sum is important it could become less meaningful in long-running programs where statistics on *recent data* (e.g., last N values or last second) is needed.
Examples of such statistics could be *interval count*, *sum* and *mean*.

Implementation of precise interval statistics is based on maintenance of recent data window which could lead to memory consumption problems.
At this point approximation of interval statistics is the solution.
But there's no such thing as a free lunch and with approximation of interval statistics we lose precision.

We base our interval statistics implementation on modification of exponential smoothing technique.

Exponential Moving Average
--------------------------

**Exponential moving average** is an example of both exponential smoothing and interval statistics.

The basic formula of exponential moving average is:

.. math::

   EMA_t = \alpha \cdot x_t + (1 - \alpha) \cdot EMA_{t - 1}


Here

- :math:`EMA_t` is exponential moving average at time :math:`t`
- :math:`x_{t - 1}` is datum at time :math:`t - 1`
- :math:`\alpha` is smoothing factor, 0 < :math:`\alpha` < 1

The only configurable parameter in exponential moving average is smoothing factor :math:`\alpha`. Higher :math:`\alpha` values will discount older data faster.

If you are interested in just about :math:`N` last data values the best choices for :math:`\alpha` would be :math:`\frac{2}{N + 1}` and :math:`\frac{1}{N}`.
With this perception exponential moving average can be roughly perceived as an interval statistics for the last N data values.

Exponential Smoothing Technique For Time Intervals
--------------------------------------------------

Enhancement of exponential smoothing could be made by adjusting smoothing factor in runtime.
Also to extend exponential smoothing to time intervals smoothing factor should be based on data timestamps and elapsed time between datum points.

For example, **interval sum** (for last time interval :math:`T`) can be maintained in the following general form (corner cases are not considered):

.. math::

   S_k = x_k + (1 - E) \cdot S_{k - 1}


Here

- :math:`S_k` is *approximately* sum of data values over last time interval :math:`T` that ends on :math:`x_k` timestamp :math:`t_k`
- :math:`x_k` is datum with timestamp :math:`t_k`
- :math:`E` is the ratio of *elapsed* time between :math:`t_{k - 1}` and :math:`t_k` timestamps to time interval :math:`T`

The *"physical" interpretation* of interval sum formula is the following.

Let :math:`S_{k - 1}` be interval sum of :math:`x` data values over last time interval :math:`T` that ends on :math:`t_{k - 1}` timestamp.
Also let sum of considered :math:`x` values be *evenly distributed* over time interval :math:`T`.

When new datum value :math:`x_k` comes at timestamp :math:`t_k` time interval window is shifted by :math:`\Delta T_k = t_k - t_{k - 1}`.
And only :math:`\frac{T - \Delta T_k}{T}` part of previous time interval window overlaps with the new one.

Thus as sum of values is evenly distributed over previous time interval only :math:`\frac{T - \Delta T_k}{T}` part of previous interval sum should preserved.
New datum value :math:`x_k` contributes to the new interval sum with no modification as evenly distributed over last :math:`\Delta T_k` time interval.

Still this approach is approximation of precise interval statistics and might behave poorly on irregular and sparse data series, but in general its behaviour is acceptable.

**Interval count** statistics is easily obtained from interval sum by replacing :math:`x` values with constant 1.

And **interval mean** is just the ratio of interval sum to interval count.

List of Handystats' Incremental Statistics
------------------------------------------

Here is the full list of incremental statistics that are supported by handystats library:

- min
- max
- sum
- count
- min
- moving average (exponential)
- interval count
- interval sum
- interval mean

Incremental Statistics Implementation
-------------------------------------

We base our incremental statistics implementation on `Boost.Accumulators <http://www.boost.org/doc/libs/1_46_0/doc/html/accumulators.html>`_
with additional moving average and interval statistics.
