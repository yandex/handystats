.. _configuration:

Configuration
=============

Handystats library may suit your needs with its default behaviour, but you might want to tune its configuration for your specific case. 

You can pass your configuration options **before** starting handystats library core (:code:`HANDY_INIT()` call) with the following methods:

- :code:`HANDY_CONFIGURATION_FILE` -- accepts file name with configuration data in JSON format.

  Example:

  .. code-block:: cpp

    HANDY_CONFIGURATION_FILE("handystats.cfg");

  and :code:`handystats.cfg` file's content:

  .. code::

    {
        "handystats": {
            "timer": {
                "idle-timeout": 100
            },
            "message-queue": {
                "sleep-on-empty": [1, 10, 100, 1000]
            }
        }
    }

- :code:`HANDY_CONFIGURATION_JSON` -- accepts string with configuration data.

  Example:

  .. code-block:: cpp

    HANDY_CONFIGURATION_JSON("\
            {\
                "handystats": {\
                    "incremental-statistics": {\
                        "moving-interval": 1000\
                    },\
                    "timer": {\
                        "idle-timeout": 1500\
                    },\
                    "json-dump": {\
                        "interval": 1000\
                    }\
                }\
            }\
        ");

- :code:`HANDY_CONFIGURATION_JSON` -- accepts :code:`rapidjson::Value` object with configuration data.

In all of these methods accepted configuration data must be in JSON format.

Here is an example of such JSON with all acceptable configuration options that should be considered as skeleton for your own configuration:

.. code-block:: javascript

    {
        "handystats": {
            "incremental-statistics": {
                "moving-average-alpha": <double value>,
                "moving-interval": <value in msec>
            },
            "timer": {
                "idle-timeout": <value in msec>
            },
            "json-dump": {
                "interval": <value in msec>
            },
            "metrics-dump": {
                "interval": <value in msec>
            },
            "message-queue": {
                "sleep-on-empty": [<first sleep interval in usec>, <second sleep interval in usec>, ...]
            }
        }
    }

Incremental Statistics Configuration
------------------------------------

Following options should be specified within :code:`"incremental-statistics"` handystats' configuration JSON entry. As an example:

.. code-block:: javascript

    {
        "handystats": {
            "incremental-statistics": {
                "moving-average-alpha": 0.25,
                "moving-interval": 1500
            }
        }
    }

Read :ref:`incremental-statistics` documentation for the backgroud of the following options.

**moving-average-alpha**
    Indirectly specifies data window length for moving average statistics.

    If you want *moving average* statistic to handle approximately last :math:`N` values
    recommended choices would be :math:`\frac{1}{N}` and :math:`\frac{2}{N + 1}`.

    *Default*: 0.125

**moving-interval**
    Specifies moving time widow length in *milliseconds* over which interval count, sum and mean
    statistics are calculated.

    *Default*: 1000

Timer Metric Configuration
--------------------------

Following options should be specified within :code:`"timer"` handystats' configuration JSON entry. As an example:

.. code-block:: javascript

    {
        "handystats": {
            "timer": {
                "idle-timeout": 5000
            }
        }
    }

Read :ref:`timer-metric` documentation for the backgroud of the following options.

**idle-timeout**
    Specifies time interval in *milliseconds* for which timer's instance is considered to be alive.

    If no events for timer's instance have been recieved during this time interval
    timer's instance will be removed with no impact on collected statistics.

    *Default*: 10000

JSON Dump Configuration
-----------------------

Following options should be specified within :code:`"json-dump"` handystats' configuration JSON entry. As an example:

.. code-block:: javascript

    {
        "handystats": {
            "json-dump": {
                "interval": 1000
            }
        }
    }

Read :ref:`json-dump` documentation for the backgroud of the following options.

**interval**
    Specifies time interval in *milliseconds* for generating JSON dump of all collected statistics.

    Zero value disables JSON dump generation.

    *Default*: 500

Metrics Dump Configuration
--------------------------

Following options should be specified within :code:`"metrics-dump"` handystats' configuration JSON entry. As an example:

.. code-block:: javascript

    {
        "handystats": {
            "metrics-dump": {
                "interval": 1000
            }
        }
    }

Read :ref:`metrics-dump` documentation for the backgroud of the following options.

**interval**
    Specifies time interval in *milliseconds* for generating metrics dump of all collected statistics.

    Zero value disables metrics dump generation.

    *Default*: 500

Message Queue Configuration
---------------------------

Following options should be specified within :code:`"message-queue"` handystats' configuration JSON entry. As an example:

.. code-block:: javascript

    {
        "handystats": {
            "message-queue": {
                "sleep-on-empty": [1, 2, 4, 8, 16]
            }
        }
    }

Read :ref:`message-queue` documentation for the backgroud of the following options.

**sleep-on-empty**
    Specifies sequence of time interval in *microseconds* for which handystats core's processing thread will sleep
    if no event messages are passed to the handystats core.

    *Default*: :code:`[1, 5, 10, 50, 100, 500, 1000, 5000, 10000]`

