#!/usr/bin/python
# Copyright (c) 2014 Yandex LLC. All rights reserved.

import json

from handystats.chrono import Timepoint
from handystats.statistics import Data

class Handystats(object):
    def __init__(self, config):
        self.config = config

        if 'metric' not in self.config or 'stat' not in self.config:
            raise RuntimeError("'metric' and 'stat' config options are mandatory")

        self.metric_name = self.config['metric']
        self.stat = self.config['stat']

    def aggregate_host(self, payload, prevtime, currtime):
        dump_timestamp, dump_str = payload.split(' ', 1)

        dump_timestamp = int(dump_timestamp) / 1000
        dump = json.loads(dump_str)

        query_interval = currtime - prevtime

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

        if self.stat == 'value':
            return merged_data.value()

        elif self.stat == 'min':
            return merged_data.min()

        elif self.stat == 'max':
            return merged_data.max()

        elif self.stat == 'sum':
            return merged_data.sum()

        elif self.stat == 'count':
            return merged_data.count()

        elif self.stat == 'avg':
            return merged_data.avg()

        elif self.stat == 'moving-count':
            return merged_data.moving_count()

        elif self.stat == 'moving-sum':
            return merged_data.moving_sum()

        elif self.stat == 'moving-avg':
            return merged_data.moving_avg()

        elif self.stat == 'quantile':
            levels = list(self.config.get('levels', [75, 90, 93, 94, 95, 96, 97, 98, 99]))

            res = []
            for level in levels:
                res.append(merged_data.quantile(level / 100.0))

            return res

        elif self.stat == 'entropy':
            return merged_data.entropy()

        elif self.stat == 'throughput':
            return merged_data.throughput()

        elif self.stat == 'frequency':
            return merged_data.frequency()

        else:
            raise NotImplementedError("'%s' statistic is not implemented" % self.stat)
