Example config
==============

```json
{
	"enable": true,

	"dump-interval": 500,

	"defaults": {
		"stats": ["count", "avg", "moving-avg", "quantile", "rate"],
		"histogram-bins": 20,
		"moving-interval": 1000,
		"rate-unit": "s"
	},

	"gauge": {
		"stats": ["moving-avg"],
		"moving-interval": 5000
	},

	"counter": {
		"stats": ["value", "moving-avg", "rate"],
		"rate-unit": "m"
	},

	"timer": {
		"stats": ["max", "moving-avg", "quantile"],
		"histogram-bins": 30,
		"moving-interval": 60000
	},

	"app.queue.[0-9].size": {
		"stats": ["max", "moving-avg"],
		"moving-interval": 1000
	},
	"app.request.{GET,UPLOAD}": {
		"stats": ["value", "moving-avg", "rate"],
		"rate-unit": "s",
		"moving-interval": 2000
	}
}
```

Description
===========

Configuration data should be provided in JSON format with handystats options in root element.

- `enable`

	Enables handystats' processing core.

	*Default*: true.

	```json
	{
		"enable": false
	}
	```

- `dump-interval`

	Sets frequency at which statistics and attributes will be dumped by processing core,
i.e. dump will be updated every `dump-interval` milliseconds.

	*Default*: 750ms

	```json
	{
		"dump-interval": 500
	}
	```

- `defaults`

	Sets default statistics options for all metrics:

	* `stats`

		Enumerates statistics that should be computed.

		*Default*: `value`, `min`, `max`, `count`, `sum`, `avg`, `moving-count`, `moving-sum`, `moving-avg`, `timestamp`

		```json
		{
			"defaults": {
				"stats": ["value", "min", "max", "moving-avg"]
			}
		}
		```

	* `moving-interval`

		Sets moving interval length in milliseconds for moving statistics
		(`moving-count`, `moving-sum`, `moving-avg`, `rate`, `histogram`, `quantile`, `entropy`).

		*Default*: 1000ms

		```json
		{
			"defaults": {
				"moving-interval": 60000
			}
		}
		```

	* `histogram-bins`

		Limits number of bins in histogram.

		*Default*: 30

		```json
		{
			"defaults": {
				"histogram-bins": 25
			}
		}
		```

	* `rate-unit`

		Sets time unit (ns, us, ms, s, m, h, d) to which `rate` statistic will be converted.

		*Default*: s

		```json
		{
			"defaults": {
				"rate-unit": "ms"
			}
		}
		```

- `gauge`

	Sets statistics options (as in `defaults`) for data produced by gauge metric.

	*Default*: as in `defaults`

	```json
	{
		"gauge": {
			"stats": ["avg", "moving-avg", "rate"],
			"moving-interval": 30000,
			"rate-unit": "m"
		}
	}
	```

- `counter`

	Sets statistics options (as in `defaults`) for data produced by counter metric.

	*Default*: as in `defaults`

	```json
	{
		"counter": {
			"stats": ["value", "min", "max", "moving-avg", "rate", "timestamp"]
		}
	}
	```

- `timer`

	- statistics options (as in `defaults`) for data produced by timer metric

		*Default*: as in `defaults`

		```json
		{
			"timer": {
				"stats": ["moving-avg", "quantile"],
				"moving-interval": 60000,
				"histogram-bins": 20
			}
		}
		```

	- `idle-timeout`

		Specifies time limit in milliseconds after which timer's instance will be dropped unless it receives heartbeat.

		*Default*: 10000ms

		```json
		{
			"timer": {
				"idle-timeout": 60000
			}
		}
		```

- patterns

	Overrides statistics options for data which name matches specified pattern

	```json
	{
		"app.queue.[0-9].size": {
			"stats": ["max", "moving-avg"],
			"moving-interval": 1000
		},
		"app.request.{GET,UPLOAD}": {
			"stats": ["value", "moving-avg", "rate"],
			"rate-unit": "s",
			"moving-interval": 2000
		}
	}
	```

Available statistics
====================

- `timestamp` - Unix timestamp in milliseconds

- *Statistics for the whole run time*:
	* `value` - last data value
	* `min` - minimum data value
	* `max` - maximum data value
	* `count` - number of data values
	* `sum` - sum of data values
	* `avg` - average of data values


- *Statistics for the last (up to `timestamp`) moving interval, estimated*:
	* `moving-count` - number of data values
	* `moving-sum` - sum of data values
	* `moving-avg` - average of data values
	* `histogram` - histogram of data values
	* `quantile` - 25-, 50-, 75-, 90-, 95-percent level quantiles ("p25", "p50", "p75", "p90", "p95") of data values
	* `rate` - rate at which data has been changed
	* `entropy` - continuous entropy of data values distribution
