import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import JsonSchemaForm from 'react-jsonschema-form';

import Modal from './Modal';
import Button from './Button';
import Spinner from './Spinner';

const Wrapper = styled.div`
  display: flex;
  flex-direction: column;
  postion: relative;
  width: 800px;

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

  height: 400px;
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
  TitleField: CustomTitleField
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
    valdate: PropTypes.func,
    onHide: PropTypes.func,
    onSubmit: PropTypes.func
  };

  static defaultProps = {
    visible: false,
    title: ""
  };

  constructor(props) {
    super(props);

    this.state = this.getStateFromProps(props);
  }

  componentWillMount() {
    this.setState(this.getStateFromProps(this.props));
  }

  componentWillReceiveProps(nextProps) {
    this.setState(this.getStateFromProps(nextProps));
  }

  getStateFromProps(props) {
    const disableSubmitButton = true;
    const formData = "formData" in props ? props.formData : this.props.formData;
    return {
      disableSubmitButton,
      formData
    };
  }

  handleSubmitButton = () => {
    this.submitButton.click();
  }

  handleChange = data => {
    const { formData, errors } = data;

    let disableSubmitButton = (errors.length !== 0);
    // I think there is a library bug React or Jsonschema
    // For workaround, I'll simply add 'formData' in setState
    this.setState({
      disableSubmitButton,
      formData
    });
  }

  render() {
    const {
      handleSubmitButton,
      handleChange
    } = this;

    const {
      disableSubmitButton
    } = this.state;

    const {
      visible,
      title,
      isLoading,
      isPending,
      schema,
      uiSchema,
      formData,
      validate,
      onHide,
      onSubmit
    } = this.props;

    return (
      <Modal 
        visible={visible} 
        onHide={onHide}>
        <Wrapper>
          <Header>
            {title}
          </Header>
          <Body>
            {isLoading && <Spinner/>}
            {!isLoading && 
              <JsonSchemaForm
                schema={schema}
                uiSchema={uiSchema}
                formData={this.state.formData}
                fields={fields}
                FieldTemplate={CustomFieldTemplate}
                liveValidate
                validate={validate}
                showErrorList={false}
                transformErrors={transformErrors}
                autocomplete="off"
                onChange={handleChange}
                onSubmit={() => onSubmit(this.state.formData)}>
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
            <Button clear onClick={onHide}>
              CANCEL
            </Button>
            <Button clear disabled={isPending || disableSubmitButton} onClick={handleSubmitButton}>
              SAVE
            </Button>
          </Footer>
        </Wrapper>  
      </Modal>
    )
  }
}

export default Form;