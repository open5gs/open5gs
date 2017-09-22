import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import JsonSchemaForm from 'react-jsonschema-form';

import Modal from './Modal';
import Button from './Button';
import Spinner from './Spinner';
import Confirm from './Confirm';

const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  postion: relative;
  width: ${p => p.width || `1050px`};

  ${media.mobile`
    width: calc(100vw - 2rem);
  `}

  background: white;
  box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
`

const Header = styled.div`
  display: flex;
  justify-content: flex-start;

  padding: 1rem;
  font-size: 1.5rem;
  background: ${oc.gray[1]};
`

const Body = styled.div`
  padding: 2rem;
  font-size: 14px;

  height: ${p => p.height || `500px`};
  ${media.mobile`
    height: calc(100vh - 16rem);
  `}

  overflow: scroll;
`

const Footer = styled.div`
  display: flex;
  justify-content: flex-end;

  padding: 1rem;
`

/* We can UI design with styled-componented. Later! */
const REQUIRED_FIELD_SYMBOL = "*";

const CustomTitleField = props => {
  const { id, title, required } = props;
  const legend = required ? title + REQUIRED_FIELD_SYMBOL : title;
  return <legend id={id}>{legend}</legend>;

};

const fields = {
  TitleField: CustomTitleField,
};

function Label(props) {
  const { label, required, id } = props;
  if (!label) {
    // See #312: Ensure compatibility with old versions of React.
    return <div />;
  }
  return (
    <label className="control-label" htmlFor={id}>
      {required ? label + REQUIRED_FIELD_SYMBOL : label}
    </label>
  );
}

const CustomFieldTemplate = props => {
  const {
    id,
    classNames,
    label,
    children,
    errors,
    help,
    description,
    hidden,
    required,
    displayLabel,
  } = props;

  if (hidden) {
    return children;
  }

  return (
    <div className={classNames}>
      {displayLabel && <Label label={label} required={required} id={id} />}
      {displayLabel && description ? description : null}
      {children}
      {errors}
      {help}
    </div>
  );
}

const transformErrors = errors => {
  return errors.map(error => {
    // use error messages from JSON schema if any
    if (error.schema.messages && error.schema.messages[error.name]) {
      return {
        ...error,
        message: error.schema.messages[error.name]
      };
    }
    return error;
  });
};

class Form extends Component {
  static propTypes = {
    visible: PropTypes.bool,
    title: PropTypes.string,
    schema: PropTypes.object,
    uiSchema: PropTypes.object,
    formData: PropTypes.object,
    isLoading: PropTypes.bool,
    valdate: PropTypes.func,
    onHide: PropTypes.func,
    onSubmit: PropTypes.func,
    onError: PropTypes.func
  };

  static defaultProps = {
    visible: false,
    title: ""
  };

  state = {};

  componentWillReceiveProps(nextProps) {
    if (this.props.visible === false && nextProps.visible === true) {
      // Initialize State Variable when form view is visible for the first time
      this.setState({ 
        formData: nextProps.formData,
        disabled: false,
        editing: false,
        confirm: false,
        disableSubmitButton: true
      })
    }
  }

  handleChange = data => {
    const {
      onChange
    } = this.props;

    let formDataChanged = null;  
    if (onChange) {
       formDataChanged = onChange(data.formData);
    }
    this.setState({
      editing: true,
      disableSubmitButton: (Object.keys(data.errors).length > 0),
      formData: formDataChanged ? formDataChanged : data.formData
    })
  }

  handleSubmit = data => {
    const {
      onSubmit
    } = this.props;

    onSubmit(data.formData);
  }

  handleSubmitButton = () => {
    this.setState({
      disabled: true,
      disableSubmitButton: true
    })
    this.submitButton.click();
  }

  handleOutside = () => {
    const {
      onHide
    } = this.props;

    if (this.state.editing === true) {
      this.setState({ confirm: true })
    } else {
      onHide();
    }
  }

  handleClose = () => {
    const {
      onHide
    } = this.props;

    this.setState({ confirm: false })
    onHide();
  }

  render() {
    const {
      handleChange,
      handleSubmit,
      handleSubmitButton,
      handleOutside,
      handleClose
    } = this;

    const {
      visible,
      title,
      schema,
      uiSchema,
      isLoading,
      validate,
      onSubmit,
      onError
    } = this.props;

    const {
      disabled,
      disableSubmitButton,
      formData
    } = this.state;

    return (
      <div>
        <Modal 
          visible={visible} 
          onOutside={handleOutside}
          disableOnClickOutside={this.state.confirm}>
          <Wrapper id='nprogress-base-form' width={this.props.width}>
            <Header>
              {title}
            </Header>
            <Body height={this.props.height}>
              {isLoading && <Spinner/>}
              {!isLoading && 
                <JsonSchemaForm
                  schema={schema}
                  uiSchema={
                    disabled ? {
                      "ui:disabled": true,
                      ...uiSchema
                    } : {
                      ...uiSchema
                    }
                  }
                  formData={formData}
                  disableSubmitButton={disableSubmitButton}
                  fields={fields}
                  FieldTemplate={CustomFieldTemplate}
                  liveValidate
                  validate={validate}
                  showErrorList={false}
                  transformErrors={transformErrors}
                  autocomplete="off"
                  onChange={handleChange}
                  onSubmit={handleSubmit}
                  onError={onError}>
                  <div>
                    <button type="submit" ref={(el => this.submitButton = el)}/>
                    <style jsx>{`
                      button {
                        display: none;
                      }
                    `}</style>
                  </div>
                </JsonSchemaForm>
              }
            </Body>
            <Footer>
              <Button clear disabled={disabled} onClick={handleClose}>
                CANCEL
              </Button>
              <Button clear disabled={disabled || disableSubmitButton} onClick={handleSubmitButton}>
                SAVE
              </Button>
            </Footer>
          </Wrapper>  
        </Modal>
        <Confirm 
          visible={this.state.confirm} 
          message="You have unsaved changes. Are you sure you want to close?"
          buttons={[
            { text: "CLOSE", action: handleClose, info:true },
            { text: "NO", action: () => this.setState({ confirm: false })}
          ]}/>
      </div>
    )
  }
}

export default Form;