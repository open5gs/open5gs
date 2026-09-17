import { Component } from 'react';
import withWidth, { SMALL } from 'helpers/with-width';
import { Form } from 'components';
const schema = {
  title: '',
  type: 'object',
  properties: {
    pei_type: {
      type: 'string',
      title: 'Equipment Identity Type*',
      enum: ['imei', 'imeisv'],
      enumNames: ['IMEI (15 digits)', 'IMEISV (16 digits)'],
      default: 'imei'
    },
    pei: {
      type: 'string',
      title: 'IMEI / IMEISV*',
      required: true,
      pattern: '^\\d+$',
      messages: { pattern: 'Only digits are allowed' }
    },
    imsi: {
      type: 'string',
      title: 'IMSI (optional subscriber rule)',
      pattern: '^\\d*$',
      messages: { pattern: 'Only digits are allowed' }
    },
    status: {
      type: 'string',
      title: 'Equipment Status*',
      required: true,
      enum: ['WHITELISTED', 'GREYLISTED', 'BLACKLISTED'],
      default: 'WHITELISTED'
    }
  }
};
class Edit extends Component {
  constructor(props) {
    super(props);
    this.state = this.stateFrom(props);
  }
  componentWillReceiveProps(props) {
    this.setState(this.stateFrom(props));
  }
  stateFrom(props) {
    const uiSchema = {};
    if (props.action === 'update')
      ['pei_type', 'pei', 'imsi'].forEach(
        (key) => (uiSchema[key] = { 'ui:disabled': true })
      );
    else if (props.width !== SMALL) uiSchema.pei = { 'ui:autofocus': true };
    return { formData: props.formData, uiSchema };
  }
  render() {
    const p = this.props;
    return (
      <Form
        visible={p.isLoading ? false : p.visible}
        title={p.action === 'update' ? 'Edit EIR Rule' : 'Create EIR Rule'}
        width="480px"
        height="440px"
        schema={schema}
        uiSchema={this.state.uiSchema}
        formData={this.state.formData}
        isLoading={p.isLoading}
        validate={p.validate}
        onHide={p.onHide}
        onSubmit={p.onSubmit}
        onError={p.onError}
      />
    );
  }
}
export default withWidth()(Edit);
