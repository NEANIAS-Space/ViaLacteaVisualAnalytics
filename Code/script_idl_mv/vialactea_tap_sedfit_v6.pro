; version v3 implements the ability to use a number of pivotal wavelength for model pre-filtering, instead of only 1
; version v5 works with new models grid version v5 delivered in early 2016
; v6 tries to implement a vector way of doing the fit rather than doing the FOR loops

function vialactea_tap_sedfit_v6,w,f,df,fflag,distance,prefilter_thresh,sed_weights=sed_weights,local=local,use_wave=use_wave,outdir=outdir,delta_chi2=delta_chi2

if (keyword_set(outdir) eq 0) then outdir=''

DB_schema='vlkb_compactsources'
DB_table='sed_models'
user='root'
db_server=''
phys_par=strupcase(['clump_mass','compact_mass_fraction','compact_mass_desired','compact_mass_actual','clump_upper_age','dust_temp','bolometric_luminosity','random_sample'])

jy2mjy=1000.

; w is the array of SED wavelengths in microns
; f is the array of the fluxes in Jy (df contains the uncertainties, if defined)
; distance is in parsec
; prefilter_thresh is the threshold for prefiltering the model grids at the given USE_WAVE wavelength

; this is an updated version with respect to FIT_SED because it only uses fixed photometric bands and extract them already at the stage of initial model selection 
; instead of re-extracting each and every pre-selected model a second time. Indeed, most of the computing time was being wasted for the MYSQL calls
d_ref=1000.
ref_wave=[3.37,3.6,4.5,4.62,5.8,8.0,12.08,22.194,24.0,70.,100.,160.,250.,350.,500.,870.,1100.]
col_names=['WISE1','I1','I2','WISE2','I3','I4','WISE3','WISE4','M1','PACS1','PACS2','PACS3','SPIR1','SPIR2','SPIR3','LABOC','BOL11']

fac_resc=(distance/d_ref)^2.
delta=1-(prefilter_thresh)

; now builds the string for mysql query to dump either the full model or the discrete sed points corresponding to fixed wavelengths
  
par_str=''
par_str_arr=strarr(n_elements(w))
ret_par_str=''
phys_str=''
phys_par_arr=strarr(n_elements(phys_par))
ret_phys_par=''  
for k=0,n_elements(w)-1 do begin
  qw=where(ref_wave-0.5 lt w(k) and ref_wave+0.5 gt w(k)) 
  par_str=par_str+col_names(qw(0))+','
  ret_par_str=ret_par_str+col_names(qw(0))+','
  par_str_arr(k)=strlowcase(col_names(qw(0)))
endfor
par_str=strupcase(strmid(par_str,0,strlen(par_str)-1))
ret_par_str=strlowcase(strmid(ret_par_str,0,strlen(ret_par_str)-1))

for k=0,n_elements(phys_par)-1 do begin
  phys_str=phys_str+phys_par(k)+','
  ret_phys_par=ret_phys_par+phys_par(k)+','
  phys_par_arr(k)=strlowcase(phys_par(k))
endfor
phys_str=strupcase(strmid(phys_str,0,strlen(phys_str)-1))
ret_phys_par=strlowcase(strmid(ret_phys_par,0,strlen(ret_phys_par)-1))

openw,unit1,outdir+'sed_returned.dat', /get_lun

if (keyword_set(use_wave)) then begin
; make sure that use_wave doe not contain bands for which e have upper limits
 for bb=0,n_elements(use_wave)-1 do begin
  qband=where(w eq use_wave(bb))
  if (qband eq -1) then begin
    print,'Reference wavelength required not found in data file.....EXIT'
;    return,-1
    break
  endif
  qrefband=where(ref_wave eq use_wave(bb))
  match,ref_wave,w,qqueryband,qdummy
  qulband=where(fflag eq 0, nqulband)
  ul_str=''
  if (nqulband gt 0) then begin
    ul_str=' and '
    for t=0,nqulband-1 do ul_str=ul_str+'('+col_names(qqueryband(qulband(t)))+"<'"+tostring(f(qulband(t))*jy2mjy*fac_resc)+"') and "
    if (fflag(qband) eq 1) then ul_str=strmid(ul_str,0,strlen(ul_str)-4)
    if (fflag(qband) eq 0) then ul_str=strmid(ul_str,4,strlen(ul_str)-4)
  endif
  
  nreq_par=1+n_elements(phys_par_arr)+n_elements(par_str_arr)
  
  
  if (fflag(qband) eq 1) then dqlcmd="select cluster_id,"+phys_str+","+par_str+" from "+db_schema+"."+db_table+" where ("+col_names(qrefband(0))+">'"+tostring(f(qband(0))*jy2mjy*fac_resc*(1-(delta^2.)))+"') and ("+col_names(qrefband(0))+" <'"+tostring(f(qband(0))*jy2mjy*fac_resc*(1+(delta^2.)))+"')"+ul_str ;fluxes are mutliplied by 1000 because model fluxes are in milliJy  
  if (fflag(qband) eq 0) then dqlcmd="select cluster_id,"+phys_str+","+par_str+" from "+db_schema+"."+db_table+" where ("+col_names(qrefband(0))+" <'"+tostring(f(qband(0))*jy2mjy*fac_resc)+"')"+ul_str ;fluxes are mutliplied by 1000 because model fluxes are in milliJy  

  curlcmd='curl --user vialactea:ia2vlkb -L --data "REQUEST=doQuery&VERSION=1.0&LANG=ADQL&FORMAT=csv&QUERY='
  curl_endpoint='" "http://palantir19.oats.inaf.it:8080/vlkb/sync?"'
  print,curlcmd+dqlcmd+curl_endpoint
  spawn,curlcmd+dqlcmd+curl_endpoint, data
  if (bb eq 0) then begin
    for i=0,n_elements(data)-1 do printf,unit1,data(i)
  endif else begin
    for i=1,n_elements(data)-1 do printf,unit1,data(i)
  endelse
 endfor      
endif else begin
; compute object luminosity from observed SED
  lum=lbol(w,f,distance)
; rescaling factor has to stay at 1 if luminosity is used-----WHYYYY ??????
;  fac_resc=1.
  dqlcmd="select cluster_id,"+phys_str+","+par_str+" from "+db_schema+"."+db_table+" where (bolometric_luminosity >'"+tostring(lum*fac_resc*(1-(delta^2.)))+"') and (bolometric_luminosity <'"+tostring(lum*fac_resc*(1+(delta^2.)))+"')" 
  curlcmd='curl --user vialactea:ia2vlkb -L --data "REQUEST=doQuery&VERSION=1.0&LANG=ADQL&FORMAT=csv&QUERY='
  curl_endpoint='" "http://palantir19.oats.inaf.it:8080/vlkb/sync?"'
  spawn,curlcmd+dqlcmd+curl_endpoint, data
  print,curlcmd+dqlcmd+curl_endpoint
  for i=0,n_elements(data)-1 do printf,unit1,data(i)
endelse
free_lun, unit1
    
;  models=read_votable8('sed_returned.dat')
dmodels=read_csv(outdir+'sed_returned.dat', header=new_tags, count=nlines)
if (nlines le 1) then return,-1
old_tags=tag_names(dmodels)
models=rename_tags(dmodels,old_tags,new_tags)

n_sel=n_elements(models.cluster_id)

;MODIFICARE PER METTERE IL CHI2 NEL FILE DI OUTPUT
openr,unit1,outdir+'sed_returned.dat', /get_lun
openw,unit2,outdir+'sedfit_output.dat', /get_lun

s=' '
readf,unit1,s
printf,unit2,s+',CHI2,DIST'

;establishes weights by assigning equal weights to mid-IR, far-IR and sub-mm
flag=fix(fflag)
ul_flag=intarr(n_elements(flag))
ul_flag(*)=0
ll_flag=intarr(n_elements(flag))
ll_flag(*)=0
dyd=fltarr(n_elements(flag))
;w1=sqrt(3./7.)
;w2=sqrt(3./7.)
;w3=sqrt(3./7.)

if (keyword_set(sed_weights) eq 0) then sed_weights=[3./7.,1./7.,3./7.] 
renorm=total(sed_weights)
w1=sqrt(sed_weights(0)/renorm)
w2=sqrt(sed_weights(1)/renorm)
w3=sqrt(sed_weights(2)/renorm)
qmir=where(w lt 25,nqmir)
qfir=where(w ge 25 and w le 250,nqfir)
qmm=where(w gt 250,nqmm)
if (nqmir gt 0) then begin
  q1=where(flag(qmir) eq 1,nq1,complement=q1neg,ncomplement=nq1neg)
  if (nq1 gt 0) then dyd(qmir(q1))=sqrt(nq1)/w1
  if (nq1neg gt 0) then begin
    dyd(qmir(q1neg))=9999.  ;i.e. it's an upper/lower limit
    ul_flag(qmir(q1neg))=1
  endif
endif
if (nqfir gt 0) then begin
  q2=where(flag(qfir) eq 1,nq2,complement=q2neg,ncomplement=nq2neg)
  if (nq2 gt 0) then dyd(qfir(q2))=sqrt(nq2)/w2
  if (nq2neg gt 0) then begin
    dyd(qfir(q2neg))=9999.   ;i.e. it's an upper limit
    ul_flag(qfir(q2neg))=1
  endif
endif
if (nqmm gt 0) then begin
  q3=where(flag(qmm) eq 1,nq3,complement=q3neg,ncomplement=nq3neg)
  if (nq3 gt 0) then dyd(qmm(q3))=sqrt(nq3)/w3
  if (nq3neg gt 0) then begin
    dyd(qmm(q3neg))=9999.   ;i.e. it's an upper limit
    ul_flag(qmm(q3neg))=1
  endif
endif

good_flag=1-ul_flag

; now normalize dyd to the minimum value
; so that we artificially increase the uncertainties of the fluxes where the SED-based weight is lower
dyd=dyd/min(dyd)
;print,'DF/F= ',dflux1/flux1
dyd=dyd*df/f

; now dyd has to be interpreted as a relative uncertainty otherwise the same assigned weight will have more or less importance depending on the level of flux and flux difference between model and observation. I then multiply the "weight" by the flux value
dyd=dyd*f

; for each model compute the adjusting distance steps
nstep=50
dist_arr=distance*(1-(delta^2.))+findgen(nstep)*(distance*(1+(delta^2.))-distance*(1-(delta^2.)))/nstep

nw=n_elements(w)
invalid_chi2=-999

matrix_models=fltarr(n_sel,nstep,nw)
matrix_chi2=fltarr(n_sel,nstep) & matrix_chi2(*,*)=invalid_chi2
matrix_fluxes=fltarr(n_sel,nstep,nw)
matrix_dfluxes=fltarr(n_sel,nstep,nw)

; create matrix of models
for i=0,nstep-1 do begin
;  print,i
  for k=0,nw-1 do z=execute('matrix_models(*,i,k)=models.'+par_str_arr(k)+'(*)/1000.*((d_ref/dist_arr(i))^2.)')
endfor

for k=0,nw-1 do matrix_fluxes(*,*,k)=f(k)
for k=0,nw-1 do matrix_dfluxes(*,*,k)=dyd(k)

print,'fatte matrici flussi'

dmat=((matrix_models-matrix_fluxes)^2)/(matrix_dfluxes^2.)

matrix_chi2=total(dmat,3)

print,'fatta matrice chi2'

;select by CHI2
if (keyword_set(delta_chi2)) then dchi2=delta_chi2 else dchi2=1

qchi2=where(matrix_chi2 le min(matrix_chi2)+dchi2, nqchi2)

par=replicate({cluster_id:' ', clump_mass:0.d0, compact_mass_fraction: 0.d0, compact_mass_desired: 0.d0, compact_mass_actual: 0.d0, clump_age:0.d0, dust_temp:0.d0, lum_bol:0.d0, sample:0, d:0.d0, chi2:0.d0, wmod:ptr_new(), fmod:ptr_new()},nqchi2)

wheretomulti,matrix_chi2,qchi2,mod_chi2,dist_chi2

print,'wheretomulti'
		
print,'selezionati ',nqchi2,' modelli'

; wmod and fmod can be addressed as, e.g., print,*(res(i).fmod)
;	readf,unit1,s
;	printf,unit2,s+','+tostring(p.chi2)+','+tostring(p.d)

    par.cluster_id=models.cluster_id(mod_chi2)
    par.clump_mass=models.clump_mass(mod_chi2)
    par.compact_mass_fraction=models.compact_mass_fraction(mod_chi2)
    par.compact_mass_desired=models.compact_mass_desired(mod_chi2)
    par.compact_mass_actual=models.compact_mass_actual(mod_chi2)
    par.clump_age=models.clump_upper_age(mod_chi2)
    par.lum_bol=models.bolometric_luminosity(mod_chi2)
    par.sample=models.random_sample(mod_chi2)
    par.dust_temp=models.dust_temp(mod_chi2)
    par.d=dist_arr(dist_chi2)
    par.chi2=matrix_chi2(qchi2)
    par.wmod=ptr_new(w)
	z=ptrarr(nqchi2)
	for i=0,nqchi2-1 do z(i)=ptr_new(matrix_models(mod_chi2(i),dist_chi2(i),*))
	par.fmod=z
	
;str_mod=strarr(nqchi2) & str_mod(*)=' '
for i=0,nqchi2-1 do begin
	str_mod=' '
	str_mod=str_mod+tostring(matrix_models(mod_chi2(i),dist_chi2(i),*))+','
	printf,unit2,tostring(par(i).cluster_id)+','+tostring(par(i).clump_mass)+','+tostring(par(i).compact_mass_fraction)+','+tostring(par(i).compact_mass_desired)+','+tostring(par(i).compact_mass_actual)+','+tostring(par(i).clump_age)+','+tostring(par(i).dust_temp)+','+str_mod+tostring(par(i).chi2)+','+tostring(par(i).d)
endfor


free_lun,unit1,unit2

return,par

end









