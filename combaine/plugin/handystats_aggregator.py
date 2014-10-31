#!/usr/bin/python
# Copyright (c) 2014 Yandex LLC. All rights reserved.

import json

from handystats.chrono import Timepoint
from handystats.statistics import Statistics, Data

class Handystats(object):
    def __init__(self, config):
        self.config = config

        if 'metric' not in self.config or 'stat' not in self.config:
            raise RuntimeError("'metric' and 'stat' config options are mandatory")

        self.metric_name = self.config['metric']
        self.stat = self.config['stat']

    def aggregate_host(self, payload):
        dump_timestamp, dump_str = payload.split(' ', 1)

        dump_timestamp = int(dump_timestamp) / 1000
        dump = json.loads(dump_str)

        # NOTE: Use 'period' config option until time frame is passed
        query_interval = self.config['period']

        if self.metric_name not in dump:
            raise RuntimeError("'%s' metric is not found" % self.metric_name)

        metric_data = Data.from_json(json.dumps(dump[self.metric_name]))
        metric_data.truncate(
            before = Timepoint.from_unixtime(dump_timestamp - query_interval),
            after = Timepoint.from_unixtime(dump_timestamp)
            )

        return metric_data.to_json()

    def aggregate_group(self, payload):
        merged_data = None

        for host_payload in payload:
            if host_payload is None:
                continue

            metric = Data.from_json(host_payload)
            if merged_data is None:
                merged_data = metric
            else:
                merged_data.merge(metric)

        if not merged_data:
            # NOTE: return 0 instead?
            raise RuntimeError("No data for metric '%s'" % self.metric_name)

        stats = Statistics.from_data(merged_data)

        if self.stat == 'value':
            return stats.value()

        elif self.stat == 'min':
            return stats.min()

        elif self.stat == 'max':
            return stats.max()

        elif self.stat == 'sum':
            return stats.sum()

        elif self.stat == 'count':
            return stats.count()

        elif self.stat == 'avg':
            return stats.avg()

        elif self.stat == 'moving-count':
            return stats.moving_count()

        elif self.stat == 'moving-sum':
            return stats.moving_sum()

        elif self.stat == 'moving-avg':
            return stats.moving_avg()

        elif self.stat == 'quantile':
            probs = list(self.config.get('probs', [75, 90, 93, 94, 95, 96, 97, 98, 99]))

            res = []
            for prob in probs:
                res.append(stats.quantile(prob / 100.0))

            return res

        else:
            raise NotImplementedError("'%s' statistic is not implemented" % self.stat)
