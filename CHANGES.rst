====================
Handystats Changelog
====================

2.0.1 (2014-12-02)
==================

Bugs fixed
----------

* Updating message queue's size statistic from client threads may lead
  to data race.

Configuration changes
---------------------

* Reserved ``"backends"`` section name.


2.0.0 (2014-11-12)
==================

Features added
--------------

* Added template methods for writing handystats' objects to ``rapidjson::Value``.

* Read/write time unit with ``handystats::chrono::time_unit_from_string()``,
  ``handystats::chrono::time_unit_as_string()``

* Added support for serialization/deserialization statistical data with
  JSON text and ``rapidjson::Value`` objects.

* Added experimental **file logger** backend.

* Added aggregation mechanics for statistical data:

  - ``append()`` method implements aggregation of consecutive data
    which represent same time-series

  - ``merge()`` method implements aggregation of independent data

* Added ``aggregate_log`` utility.

* Added python bindings for aggregation mechanics.

* Added new statistics:

  - **throughput** -- moving sum of data values averaged to second

  - **frequency** -- moving count of data values averaged to second

* Added ``HANDY_CONFIG_ERROR()`` method to access error message occurred
  during configuration application.

* Added support for ``CLOCK_MONOTONIC_RAW`` clock source.

Bugs fixed
----------

* TSC rate determination fixed which may lead to tests failure due to
  TSC rate inaccuracy.

* Number of global objects radically reduced.

  During program termination without calling ``HANDY_FINALIZE()``
  order-of-destruction problem may occur and lead to abnormnal termination.

* Exceptions thrown from configuration methods ``HANDY_CONFIG_JSON()`` and
  ``HANDY_CONFIG_FILE()``.

Configuration changes
---------------------

* Configuration format simplified:

  - ``"statistics"`` -> ``"defaults"``

  - ``"core.enable"`` -> ``"enable"``

  - ``"tags"`` -> ``"stats"``

  - ``"metrics-dump.interval"`` -> ``"dump-interval"``

  - ``"metrics"`` section removed.

  - ``"gauge"``, ``"counter"``, ``"timer"`` sections moved to the top level.

* Added pattern statistics configuration.

  Examples of pattern names:

  - app.queue.[0-9].size

  - app.request.{GET,UPLOAD}


Other changes
-------------

* Common map of metrics in dump splitted into two maps -- metrics and attributes.

* Metrics objects replaced with corresponding statistics objects in metrics dump.

* Removed rapidjson headers from public library headers.

* Removed support for ``rapidjson::Value`` in HANDY_CONFIG_JSON method.

* Attribute **handystats.dump_timestamp** in dump replaced with:

  - **handystats.system_timestamp** represents unix timestamp in milliseconds to
    which current dump is related.

  - **handystats.internal_timestamp** represents unix timestamp in milliseconds
    to which user metrics' state is related.

* Removed **rate** statistic.

* Removed deprecated statistic extraction interface (``min()``, ``max()``, etc.).
