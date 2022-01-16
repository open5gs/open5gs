import unittest
import pymongo

import SchemaUpdater

class TestSchemaUpdater(unittest.TestCase):
    def setUp(self):
        self.legacy_sub = {
            'imsi': '999990000000001',
            'access_restriction_data': 32,
            'ambr': {
                'downlink': 1024000,
                'uplink': 1024000
            },
            'network_access_mode': 0,
            'pdn': [
                {
                    'apn': 'internet',
                    'pcc_rule': [],
                    'ambr': {
                        'downlink': 1024000,
                        'uplink': 1024000
                    },
                    'qos': {
                        'qci': 9,
                        'arp': {
                            'priority_level': 8,
                            'pre_emption_vulnerability': 1,
                            'pre_emption_capability': 0
                        }
                    },
                    'type': 0,
                    'ue': {
                        'addr': '10.45.1.1',
                        'addr6': 'dead:beef::1'
                    }
                }
            ],
            'security': {
                'k': 'iamatransparentsecretkeystringk',
                'amf': '8000',
                'op': None,
                'opc': 'iamatransparentsecretopcstring'
            },
            'subscribed_rau_tau_timer': 12,
            'subscriber_status': 0
        }

    def test_top_level_migration(self):
        new_sub = SchemaUpdater.create_v1_from_v0(self.legacy_sub)
        self.assertEqual(new_sub["imsi"], self.legacy_sub["imsi"])
        self.assertEqual(new_sub["subscriber_status"], self.legacy_sub["subscriber_status"])
        self.assertEqual(new_sub["subscribed_rau_tau_timer"], self.legacy_sub["subscribed_rau_tau_timer"])
        self.assertEqual(new_sub["network_access_mode"], self.legacy_sub["network_access_mode"])
        self.assertEqual(new_sub["access_restriction_data"], self.legacy_sub["access_restriction_data"])

    def test_ambr_migration(self):
        new_sub = SchemaUpdater.create_v1_from_v0(self.legacy_sub)
        self.assertEqual(
            new_sub["ambr"]["uplink"]["value"] * (1000 ** new_sub["ambr"]["uplink"]["unit"]),
            self.legacy_sub["ambr"]["uplink"]
        )

        self.assertEqual(
            new_sub["ambr"]["downlink"]["value"] * (1000 ** new_sub["ambr"]["downlink"]["unit"]),
            self.legacy_sub["ambr"]["downlink"]
        )

    def test_pdn_migration(self):
        new_sub = SchemaUpdater.create_v1_from_v0(self.legacy_sub)
        self.assertEqual(len(new_sub["slice"]), 1)
        self.assertEqual(len(new_sub["slice"][0]["session"]), len(self.legacy_sub["pdn"]))

        session = new_sub["slice"][0]["session"][0]
        self.assertEqual(
            session["ambr"]["uplink"]["value"] * (1000 ** session["ambr"]["uplink"]["unit"]),
            self.legacy_sub["pdn"][0]["ambr"]["uplink"]
        )

        self.assertEqual(
            session["ambr"]["downlink"]["value"] * (1000 ** session["ambr"]["downlink"]["unit"]),
            self.legacy_sub["pdn"][0]["ambr"]["downlink"]
        )

        self.assertEqual(
            session["ue"]["addr"],
            self.legacy_sub["pdn"][0]["ue"]["addr"]
        )

        self.assertEqual(
            session["ue"]["addr6"],
            self.legacy_sub["pdn"][0]["ue"]["addr6"]
        )
