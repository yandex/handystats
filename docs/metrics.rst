.. _metrics:

Metrics
============

We see **metrics** and **statistics** on different levels of abstraction.
While *statistics* (in particular, :ref:`incremental-statistics`) characterize time-stamped data series whitout any relations with another series,
*metrics* are data sources for statistics and can aggragate related statistics.

**Metrics** are an interface to represent internal processes and inherent data in user's application.
And metrics' collected statistics provide user an opportunity to understand application's state.

Examples of metrics presented in handystats library are:

- counters
- timers
- gauges


Counters
--------

**Counter** is just an integer that starts with initial value (often it's zero) and can be incremented and decremented by delta values.

Counters depsite and due to its simplicity are one of the most widely used type of metrics.

For example, it can represent number of active users, size of internal queue in user's application and a lot more other data
with the only restriction that data change by deltas.
If you want to track data that change by its value see :ref:`metrics-gauge`.

.. rubric:: What Statistics Counter Can Provide?

It's obvious that *last value* of counter is pretty much meaningless and gives your no information about counter's previous states and its behaviour.

Therefore, in addition to **last value** and **last timestamp** of counter we provide the following statistics:

- **values**

  This statistics collect values through which counter passes in between its changes by deltas.

  For example, if counter starts with 0 as initial value and then changes by the following deltas: :code:`1, 2, -3, 5, 10`.

  Then values through which the counter has passed will be: :code:`0, 1, 3, 0, 5, 15`.

- **increasing deltas**

  This statistics collect only positive deltas by which counter changeses.

  As in above example, positive deltas will be: :code:`1, 2, 5, 10`.

- **decreasing deltas**

  This statistics collect absolute values of only negative deltas by which counter changeses.

  As in above example, positive deltas will be: :code:`3`.

- **deltas**

  This statistics collect all deltas by which counter changeses.

  As in above example, deltas will be: :code:`1, 2, -3, 5, 10`.

Aggregation of all of the above data (values, increasing deltas, decreasing deltas, deltas) are performed by incremental statistcs.
See :ref:`incremental-statistics` for more details.

Timers
------

**Timer** allows you to measure time spans durations and collects statistics about these duration values.

Time spans measured by particular timer can *overlap* with each other.
For example, it can be time spans measured from different threads or time spans between events' entrance and its processing completion.

See :ref:`chrono` for more details on how we measure time spans and why it's *highly accurate*.

.. rubric:: Timer's Instances

To support overlapping time spans by single timer we introduce the term of **timer's instance**.

Timer's instance marked with *unique ID within single timer* defines currently active time span.
When this active time span ends timer's instance measures total time span's duration and updates collected timer statistics.
After that corresponding timer's instance is removed from the timer and its unique ID becomes free for subsequent use.

.. rubric:: Timer's Timeout

Sometimes user might forget to send final event for particular timer's instance or measured event took too long time for completion.

Anyway, we can't allow timer's instance to live forever.

Timer's instance is allowed to live with no events for this particular instance up to **idle timeout**.
If no events have been received by timer's instance during this time interval, timer's instance is considered dead and is removed from the timer with no impact on collected statistics.

You can configure this option, see :ref:`configuration` for more details.

But if you want to extend lifetime of particular timer's instance you should send **heartbeat events** during measured time interval
to tell handystats library core that timer's instance is alive.

.. rubric:: What Statistics Timer Can Provide?

As with counter's statistics only **last time span's duration** and **last timestamp** are not enough.

Thus handystats library provide statistics about all **values** collected by timer -- durations of all correctly finished timer's instances.

Aggregation of collected values are performed by incremental statistics. See :ref:`incremental-statistics` for more details.

.. _metrics-gauge:

Gauges
------

**Gauge** is the simplest type of statistics.
It collects only *values* that you pass to the metric.

For example, you might want to measure size of some structure that updates without your influence.
At this point you can request size of the structure from time to time and pass requested values to the gauge metric.

Underlying data aggregation is exactly incremental statistics. See :ref:`incremental-statistics` for more details.
