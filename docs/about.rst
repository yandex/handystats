.. _about:

About
=====

What Is Handystats?
---------------------------

Handystats is C++ library for collecting **user-defined in-process runtime statistics** with **low overhead**
that allows users to monitor their applications in a production environment.

**What Does It Mean, Exactly?**

- By **statistics** we mean a collection of metrics (counters, timers, gauges, etc.) and its cumulative data such as min, max, sum, count, etc.
- By **runtime** we mean that we're aimed to collect and provide statistics about program's execution in "near real-time".
- By **in-process** we mean that statistics' collection and aggregation are performed within the program's process (but still in separate thread).
- By **user-defined** we mean that you must instrument your own code with metrics that should be collected.
- By **low overhead** we mean that library's influence on application's performance must be minimal.

What Problem We Are Aimed To Solve?
-----------------------------------

Our main goal is to provide you with an ability to "*look inside*" your own program at runtime via collected statistics (which then can be displayed as numbers, graphics, etc.) and thus better understand what is going on.

Also we can provide your program with gathered statistics that can be used to adjust its own behaviour at runtime.

Motivation
----------

Imaging how simple life would be if all programs were black boxes with number of inputs and outputs that just work.
But it's almost never true despite any number of testing stages that this black box had passed.
Thus administrator and/or programmer should have to know what and why is happening inside that black box.

Logs could help with that, but logs are *raw data* which means someone should analyze them to produce meaningful information.
And we're not mentioning their disk space consumption.

At this point runtime statistics collected and accumulated *inside* the program looks as more suitable solution.
Programmer could implement counters, timers and other metrics by hand, but it's a fair amount of work considering such requirements as low overhead and support for multi-threaded program.
And this should definitely not be implemented again every time one starts new project.

Inspiring Example
-----------------

Consider a program that processes input events and sends processing result further:

.. code-block:: cpp

    void process(event_type event) {
        result = internal_process(event);
        send_process_result(result);
    }

Here is the black box! We know neither input events count and rate nor processing and sending timings.
With slightly modifications we can tell handystats library to collect such metrics:

.. code-block:: cpp

    void process(event_type event) {
        HANDY_COUNTER_INCREMENT("events count", 1);

        HANDY_TIMER_START("internal process time");
        result = internal_process(event);
        HANDY_TIMER_STOP("internal process time");

        HANDY_TIMER_START("send result time");
        send_process_result(result);
        HANDY_TIMER_STOP("send result time");
    }

Here we ask handystats library to increment counter with name ``"events count"`` by 1 on each new event.
Measurement of ``internal_process`` and ``send_process_result`` running times is performed by starting and stopping coresponding timers.

Now handystats library will collect statistics about these metrics.
Then we can request collected statistics in, for example, JSON format:

.. code-block:: javascript

    {
        "events count": {
            "type": "counter",
            "timestamp": ...,
            "value": 10000,
            "values": {
                "min": 0,
                "max": 10000,
                "mean": 5000,
                ...
            },
            "incr-deltas": {
                ...
            },
            "decr-deltas": {
                ...
            },
            ...
        },
        "internal process time": {
            "type": "timer",
            "timestamp": ...,
            "value": 300,
            "values": {
                "min": 230,
                "max": 560,
                "mean": 320.56,
                ...
            },
            ...
        },
        "send result time": {
            ...
        },
        ...
    }

Here "events count" counter entry contains information about:

- its type
- last timestamp
- last value
- cumulative statistics about all counter values, increasing deltas, decreasing deltas
- ...

And "internal process time" as well as "send result time" timers entries contain information about:

- its type
- last timestamp
- last value
- cumulative statistics about all timer values
- ...

