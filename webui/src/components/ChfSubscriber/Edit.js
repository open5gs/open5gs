/*
 * CHF subscriber quota profile — Edit / Create form.
 *
 * The user enters ONE total data quota (UL + DL combined).  The CHF grants
 * grantedUnit.totalVolume so the UPF deducts both uplink and downlink octets
 * from a single counter, ensuring UL+DL never exceeds the configured limit.
 *
 * Field → MongoDB mapping (after Document.js / formToDb conversion):
 *   totalVolume + volumeUnit  → quota.totalQuotaBytes  (Number, bytes)
 *   validityPeriod            → quota.validityPeriod   (String enum)
 *   validitySeconds           → quota.validitySeconds  (Number, seconds; resolved from preset)
 *   autoRenew                 → quota.autoRenew        (Boolean)
 *   finalUnitAction           → quota.finalUnitAction  (String enum)
 *
 * 3GPP: TS 32.291 §6.3.1.2 — GrantedUnit / FinalUnitIndication
 *       TS 32.291 §6.3.2.2 — TIME_LIMIT trigger / validityTime
 */

import { Component } from 'react';
import PropTypes from 'prop-types';

import withWidth, { SMALL } from 'helpers/with-width';
import { Form } from 'components';

const schema = {
  title: '',
  type: 'object',
  required: ['supi'],
  properties: {
    supi: {
      type: 'string',
      title: 'SUPI*',
      pattern: '^imsi-\\d{15}$',
      messages: {
        pattern: 'Must be: imsi- followed by exactly 15 digits (e.g. imsi-999700000000001)',
      },
    },
    /*
     * What this allowance covers.  Every field is optional, and one left
     * empty is open: it matches anything.  A bucket with nothing filled in
     * is the subscriber's catch-all, which is what a plain per-SUPI plan
     * is.  The CHF matches these against the charging request (TS 32.291):
     * the slice from networkSlicingInfo.sNSSAI, the DNN from dnnId, the
     * home network from hPlmnId, roaming from userInformation.roamerInOut.
     */
    scopeForm: {
      type: 'object',
      title: 'Applies To',
      properties: {
        sst: {
          type: 'string',
          title: 'Slice SST',
          pattern: '^$|^([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$',
          messages: { pattern: 'Leave empty for any slice, or enter 0-255' },
        },
        sd: {
          type: 'string',
          title: 'Slice SD',
          pattern: '^$|^[0-9a-fA-F]{6}$',
          messages: { pattern: 'Leave empty for any, or 6 hex digits (e.g. 010203)' },
        },
        dnn: {
          type: 'string',
          title: 'DNN',
        },
        hplmn: {
          type: 'string',
          title: 'Home PLMN',
          pattern: '^$|^\\d{3}-\\d{2,3}$',
          messages: { pattern: 'Leave empty for any, or MCC-MNC (e.g. 999-70)' },
        },
        roaming: {
          type: 'string',
          title: 'Roaming',
          enum: ['', 'home', 'inBound', 'outBound'],
          enumNames: ['Any', 'Home network only', 'Inbound roamers',
                      'Roaming abroad'],
          default: '',
        },
      },
    },

    priority: {
      type: 'integer',
      title: 'Priority',
      default: 0,
    },

    quota: {
      type: 'object',
      title: 'Quota Profile',
      properties: {

        /*
         * Total data allowance entered by the user.
         * Document.js / formToDb() converts this to totalQuotaBytes (bytes).
         * The CHF grants grantedUnit.totalVolume; the UPF deducts UL+DL
         * bytes from a single counter so total consumption never exceeds this.
         */
        totalVolume: {
          type: 'number',
          title: 'Total Data Quota',
          default: 10,
          minimum: 0,
        },

        /*
         * Unit for totalVolume — MB, GB, or TB (binary prefixes, IEC 80000-13).
         * formToDb() multiplies totalVolume by the appropriate factor.
         *   1 MB = 1 048 576 bytes   (2^20)
         *   1 GB = 1 073 741 824 bytes (2^30)
         *   1 TB = 1 099 511 627 776 bytes (2^40)
         */
        volumeUnit: {
          type: 'string',
          title: 'Unit',
          enum: ['MB', 'GB', 'TB'],
          default: 'GB',
        },

        /*
         * Billing cycle preset.  The WebUI resolves this to validitySeconds
         * before persisting; the C layer reads validitySeconds only.
         *   daily   →  86 400 s (24 h)
         *   weekly  → 604 800 s (7 d)
         *   monthly → 2 592 000 s (30 d)
         *   custom  → use the validitySeconds field below
         */
        validityPeriod: {
          type: 'string',
          title: 'Validity Period',
          enum: ['daily', 'weekly', 'monthly', 'custom'],
          enumNames: ['Daily (24 h)', 'Weekly (7 d)', 'Monthly (30 d)', 'Custom'],
          default: 'monthly',
        },

        /*
         * Custom period in seconds.  Only used when validityPeriod === 'custom'.
         * For preset periods, formToDb() ignores this field and derives the
         * canonical seconds from the PERIOD_SECONDS table.
         */
        validitySeconds: {
          type: 'integer',
          title: 'Custom Period (seconds)',
          default: 2592000,
          minimum: 1,
        },

        /*
         * Auto-renew on period expiry.
         *   true  — the CHF resets accumulated usage and re-grants the full
         *           quota at the start of each new period (recurring billing).
         *   false — on period expiry the session is terminated according to
         *           the finalUnitAction configured below.
         */
        autoRenew: {
          type: 'boolean',
          title: 'Auto-Renew on Expiry',
          default: true,
        },

        /*
         * Action when quota is volume-exhausted before the period expires.
         * TS 32.291 §7.3:
         *   TERMINATE       — terminate the PDU session (default, safest)
         *   REDIRECT        — redirect traffic to a captive portal
         *   RESTRICT_ACCESS — allow only a restricted service set
         */
        finalUnitAction: {
          type: 'string',
          title: 'Final Unit Action',
          enum: ['TERMINATE', 'REDIRECT', 'RESTRICT_ACCESS'],
          default: 'TERMINATE',
        },
      },
    },
  },
};

const uiSchema = {
  supi: {
    'ui:placeholder': 'imsi-001010000000001',
  },
  scopeForm: {
    sst: {
      'ui:placeholder': 'any',
      'ui:help': 'Slice/Service Type. Leave empty to cover every slice.',
    },
    sd: {
      'ui:placeholder': 'any',
      'ui:help': 'Slice Differentiator, 6 hex digits. Empty covers every instance of the SST.',
    },
    dnn: {
      'ui:placeholder': 'any',
      'ui:help': 'Data Network Name, e.g. internet. Empty covers every DNN. ims, sos, mcptt and mcx are never quota-managed, so a bucket scoped to them is never charged.',
    },
    hplmn: {
      'ui:placeholder': 'any',
      'ui:help': "The subscriber's home network as MCC-MNC. Empty covers every PLMN.",
    },
    roaming: {
      'ui:help': 'Restrict this allowance to sessions at home, or while roaming.',
    },
  },
  priority: {
    'ui:help': 'When several allowances cover one session, the highest priority wins. Equal numbers keep the older one.',
  },
  quota: {
    totalVolume: {
      'ui:help': 'Total data allowance (UL + DL combined). The CHF shares this dynamically between uplink and downlink.',
    },
    volumeUnit: {
      'ui:help': 'MB = 1 048 576 bytes · GB = 1 073 741 824 bytes · TB = 1 099 511 627 776 bytes (IEC 80000-13).',
    },
    validityPeriod: {
      'ui:help': 'Billing cycle length. Choose "Custom" to enter an arbitrary number of seconds.',
    },
    validitySeconds: {
      'ui:help': 'Used only when "Custom" period is selected. Ignored for daily / weekly / monthly presets.',
    },
    autoRenew: {
      'ui:help': 'When enabled, the CHF resets usage and re-grants the full quota at the start of each new period.',
    },
    finalUnitAction: {
      'ui:help': 'Action taken when the volume quota is exhausted before the period expires.',
    },
  },
};

/* A pool form: a new pool, or an existing one, which has members. */
function isPoolForm(action, formData) {
  return action === 'create-pool' ||
      !!(formData && Array.isArray(formData.members));
}

function titleFor(action, formData) {
  if (action === 'create-pool') return 'Create Shared Pool';
  if (isPoolForm(action, formData))
    return `Edit Shared Pool ${(formData && formData.bucketId) || ''}`.trim();
  return action === 'update' ? 'Edit CHF Quota Profile' : 'Create CHF Quota Profile';
}

class Edit extends Component {
  static propTypes = {
    visible: PropTypes.bool,
    action: PropTypes.string,
    formData: PropTypes.object,
    isLoading: PropTypes.bool,
    validate: PropTypes.func,
    onHide: PropTypes.func,
    onSubmit: PropTypes.func,
    onError: PropTypes.func,
  };

  constructor(props) {
    super(props);
    this.state = this.getStateFromProps(props);
  }

  componentWillReceiveProps(nextProps) {
    this.setState(this.getStateFromProps(nextProps));
  }

  getStateFromProps(props) {
    const { action, width, formData } = props;

    let localUiSchema = Object.assign({}, uiSchema);

    /*
     * A shared pool: members instead of a SUPI, one per line.  Saved through
     * the TMF API (Document.js), which keeps a SIM to one pool, applies the
     * member limit from chf.yaml, and releases a removed SIM's sessions on
     * the pool at once.  A new pool needs two SIMs at least - one SIM is a
     * SIM's own allowance - and an existing one keeps one at least.
     */
    if (isPoolForm(action, formData)) {
      const create = action === 'create-pool';
      const properties = Object.assign({
        membersText: {
          type: 'string',
          title: 'SIMs sharing this pool*',
          pattern: create
            ? '^\\s*imsi-\\d{15}([\\s,]+imsi-\\d{15})+\\s*$'
            : '^\\s*imsi-\\d{15}([\\s,]+imsi-\\d{15})*\\s*$',
          messages: {
            pattern: create
              ? 'List two SIMs or more, one per line, each imsi- and 15 digits'
              : 'List one SIM or more, one per line, each imsi- and 15 digits',
          },
        },
      }, schema.properties);
      delete properties.supi;
      return {
        schema: Object.assign({}, schema,
            { required: ['membersText'], properties }),
        uiSchema: Object.assign({}, localUiSchema, {
          membersText: {
            'ui:widget': 'textarea',
            'ui:autofocus': create && width !== SMALL,
            'ui:placeholder': 'imsi-999700000000001\nimsi-999700000000002',
            'ui:help': 'One SIM per line. A SIM can be in one pool only, ' +
              'and a pool can have as many SIMs as charging.pool.max_members ' +
              'in chf.yaml allows (10 as shipped). A SIM you remove has its ' +
              'sessions on this pool released at once. At equal priority a ' +
              "SIM's own allowance is used before the pool.",
          },
        }),
        formData,
      };
    }

    if (action === 'update') {
      localUiSchema = Object.assign({}, localUiSchema, {
        supi: { 'ui:disabled': true },
      });
    } else if (width !== SMALL) {
      localUiSchema = Object.assign({}, localUiSchema, {
        supi: { 'ui:autofocus': true },
      });
    }

    return { schema, uiSchema: localUiSchema, formData };
  }

  render() {
    const {
      visible,
      action,
      isLoading,
      validate,
      onHide,
      onSubmit,
      onError,
    } = this.props;

    const { formData } = this.state;

    return (
      <Form
        visible={isLoading ? false : visible}
        title={titleFor(action, this.state.formData)}
        width="560px"
        height="640px"
        schema={this.state.schema}
        uiSchema={this.state.uiSchema}
        formData={formData}
        isLoading={isLoading}
        validate={validate}
        onHide={onHide}
        onSubmit={onSubmit}
        onError={onError}
      />
    );
  }
}

export default withWidth()(Edit);
