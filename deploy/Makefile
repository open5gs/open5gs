build-for-prod:
	ansible-playbook deploy-gtp-proxy.yml --tags build

deploy-into-pwg:
	ansible-playbook -i hosts_prod $(ANSIBLE_OPTS) deploy-gtp-proxy.yml

restore-prod:
	ansible-playbook -i hosts_prod $(ANSIBLE_OPTS) restore-pwg.yml

.PHONY: build-for-prod deploy-prod restore-prod
